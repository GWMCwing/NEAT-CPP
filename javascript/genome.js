/**
 * Genome class.
 * @class
 * @constructor
 * @public
 */
class Genome {
	/**
	 *
	 * @param {number} input number of input nodes
	 * @param {number} output number of output nodes
	 * @param {number} id id of the genome
	 * @param {boolean} offSpring ???
	 */
	constructor(input, output, id, offSpring = false) {
		// Genome parameters
		this.inputs = input;
		this.outputs = output;
		this.id = id;
		this.layers = 2;
		this.nextNode = 0;

		this.nodes = [];
		this.connections = [];

		if (!offSpring) {
			for (let i = 0; i < input; i++) {
				this.nodes.push(new Node(this.nextNode, 0));
				this.nextNode++;
			}

			for (let i = 0; i < output; i++) {
				this.nodes.push(new Node(this.nextNode, 1, true));
				this.nextNode++;
			}

			for (let i = 0; i < input; i++) {
				for (let j = input; j < output + input; j++) {
					const weight =
						Math.random() * this.inputs * Math.sqrt(2 / this.inputs);
					this.connections.push(
						new Connection(this.nodes[i], this.nodes[j], weight)
					);
				}
			}
		}
	}

	// Network Core
	generateNetwork() {
		// Clear all output Connection in the nodes
		const nodesLen = this.nodes.length;
		for (let i = 0; i < nodesLen; i++) {
			const node = this.nodes[i];
			node.outputConnections.splice(0, node.outputConnections.length);
		}
		// add the connections to the nodes
		const connectionsLen = this.connections.length;
		for (let i = 0; i < connectionsLen; i++) {
			const connection = this.connections[i];
			connection.fromNode.outputConnections.push(connection);
		}
		// prepare for feed forward
		this.sortByLayer();
	}

	/**
	 *
	 * @param {number[]} inputs array of inputs
	 * @returns {number[]} array of outputs
	 */
	feedForward(inputs) {
		this.generateNetwork(); // connect all the nodes

		// clear old inputs
		const nodesLen = this.nodes.length;
		for (let i = 0; i < nodesLen; i++) {
			this.nodes[i].inputSum = 0;
		}
		//assign new inputs
		for (let i = 0; i < this.inputs; i++) {
			this.nodes[i].outputValue = inputs[i];
		}
		// engage all nodes and Extract the results from the output nodes
		const result = [];
		for (let i = 0; i < nodesLen; i++) {
			const node = this.nodes[i];
			node.engage();
			if (node.output) {
				result.push(node.outputValue);
			}
		}
		return result;
	}

	/**
	 * crossover
	 * @param {Genome} partner
	 *
	 * @returns {Genome} offspring genome
	 */
	crossover(partner) {
		//TODO unique id
		const offSpring = new Genome(this.inputs, this.outputs, 0, true); // Child genome
		offSpring.nextNode = this.nextNode;

		// Take all nodes from this parent - output node activation 50% - 50%
		const nodesLen = this.nodes.length;
		for (let i = 0; i < nodesLen; i++) {
			const node = this.nodes[i].clone();
			if (node.output) {
				const partnerNode = partner.nodes[partner.getNode(node.number)];
				if (Math.random() < 0.5) {
					node.activationFunction = partnerNode.activationFunction;
					node.bias = partnerNode.bias;
				}
			}
			offSpring.nodes.push(node);
		}

		// Randomly take connections form this or the partner network
		//? never used
		const maxLayer = 0;
		const connectionLen = this.connections.length;
		for (let i = 0; i < connectionLen; i++) {
			const index = this.commonConnection(
				this.connections[i].getInnovationNumber(),
				partner.connections
			);

			if (index != -1) {
				// have common connections

				const conn =
					Math.random() < 0.5
						? this.connections[i].clone()
						: partner.connections[index].clone();

				// Reassign nodes
				const fromNode =
					offSpring.nodes[offSpring.getNode(conn.fromNode.number)];
				const toNode = offSpring.nodes[offSpring.getNode(conn.toNode.number)];

				conn.fromNode = fromNode;
				conn.toNode = toNode;

				// Add this connection to the child
				if (fromNode && toNode) {
					offSpring.connections.push(conn);
				}
			} else {
				// no common connection => take from this
				const conn = this.connections[i].clone();

				// Reassign nodes
				const fromNode =
					offSpring.nodes[offSpring.getNode(conn.fromNode.number)];
				const toNode = offSpring.nodes[offSpring.getNode(conn.toNode.number)];

				conn.fromNode = fromNode;
				conn.toNode = toNode;

				// add this connection to the child
				if (fromNode && toNode) {
					offSpring.connections.push(conn);
				}
			}
		}
		offSpring.layers = this.layers;
		return offSpring;
	}

	// Mutation
	mutate() {
		// let mut;
		//? not exclusive event?

		if (Math.random() < 0.8) {
			// modify connection
			// mut = 'ModConn';
			for (let i = 0; i < this.connections.length; i++) {
				this.connections[i].mutateWeight();
			}
		}
		if (Math.random() < 0.5) {
			// modify bias
			// mut = 'ModBias';
			for (let i = 0; i < this.nodes.length; i++) {
				this.nodes[i].mutateBias();
			}
		}
		if (Math.random() < 0.1) {
			// modify activation function
			// mut = 'ModAct';
			let i = Math.floor(Math.random() * this.nodes.length);
			this.nodes[i].mutateActivation();
		}
		if (Math.random() < 0.01) {
			// add mode
			// mut = 'AddNode';
			this.addNode();
		}
	}

	addNode() {
		// add node to network
		// get a random connection to replace with a node
		let connectionIndex = Math.floor(Math.random() * this.connections.length);
		let pickedConnection = this.connections[connectionIndex];
		pickedConnection.enabled = false;
		// delete connection
		this.connections.splice(connectionIndex, 1);

		// create new node
		let newNode = new Node(this.nextNode, pickedConnection.fromNode.layer + 1);
		for (let i = 0; i < this.nodes.length; i++) {
			const node = this.nodes[i];
			if (node.layer > pickedConnection.fromNode.layer) {
				node.layer++;
			}
		}

		// new connections
		let newConnection1 = new Connection(pickedConnection.fromNode, newNode, 1); // set weight as 1
		let newConnection2 = new Connection(
			newNode,
			pickedConnection.toNode,
			pickedConnection.weight
		);
		this.layers++;
		this.connections.push(newConnection1);
		this.connections.push(newConnection2);
		this.nodes.push(newNode);
		this.nextNode++;
	}

	addConnection() {
		if (this.fullyConnected()) {
			return;
		}

		// choose nodes to connect
		let node1Index = Math.floor(Math.random() * this.nodes.length);
		let node2Index = Math.floor(Math.random() * this.nodes.length);
		// ensure nodes are valid, not the same and not connected
		//TODO factorize
		while (
			this.nodes[node1Index].layer == this.nodes[node2Index].layer ||
			this.nodesConnected(this.nodes[node1Index], this.nodes[node2Index])
		) {
			node1Index = Math.floor(Math.random() * this.nodes.length);
			node2Index = Math.floor(Math.random() * this.nodes.length);
		}

		// switch nodes based on their layers, ensure node1 is always on the left
		if (this.nodes[node1Index].layer > this.nodes[node2Index].layer) {
			let temp = node1Index;
			node1Index = node2Index;
			node2Index = temp;
		}

		// add connection
		let newConnection = new Connection(
			this.nodes[node1Index],
			this.nodes[node2Index],
			Math.random() * this.inputs * Math.sqrt(2 / this.inputs)
		);
		this.connections.push(newConnection);
	}

	// Utilities

	// search through all connections to check for one with the correct Innovation number
	commonConnection(innN, connections) {
		for (let i = 0; i < connections.length; i++) {
			if (innN == connections[i].getInnovationNumber()) {
				return i;
			}
		}
		// not found
		return -1;
	}

	// search if there is a connection between node1 and node2
	nodesConnected(node1, node2) {
		for (let i = 0; i < this.connections.length; i++) {
			const conn = this.connections[i];
			if (
				(conn.fromNode == node1 && conn.toNode == node2) ||
				(conn.fromNode == node2 && conn.toNode == node1)
			) {
				return true;
			}
		}
		return false;
	}

	// check if the network is fully connected
	fullyConnected() {
		let maxConnections = 0;
		let nodesPerLayer = [];

		// Calculate all possible connections
		for (let i = 0; i < this.nodes.length; i++) {
			const node = this.nodes[i];
			if (nodesPerLayer[node.layer] != undefined) {
				nodesPerLayer[node.layer]++;
			} else {
				nodesPerLayer[node.layer] = 1;
			}
		}
		for (let i = 0; i < this.layers - 1; i++) {
			for (let j = i + 1; j < this.layers; j++) {
				maxConnections += nodesPerLayer[i] * nodesPerLayer[j];
			}
		}
		// compare
		return maxConnections == this.connections.length;
	}
	// sort all nodes by layer
	sortByLayer() {
		this.nodes.sort((a, b) => a.layer - b.layer);
	}
	// return a copy of this genome
	clone() {
		let clone = new Genome(this.inputs, this.outputs, this.id);
		clone.nodes = this.nodes.slice(0, this.nodes.length);
		clone.connections = this.connections.slice(0, this.connections.length);
		return clone;
	}
	// returns the index of a node with that number
	getNode(x) {
		for (let i = 0; i < this.nodes.length; i++) {
			if (this.nodes[i].number == x) {
				return i;
			}
		}
		return -1;
	}

	// computational weight of the network
	calculateWeight() {
		return this.connections.length + this.nodes.length;
	}

	draw(width = 400, height = 400, container = 'svgContainer') {
		//Draw the genome to a svg
		var element = document.getElementById(this.id);
		if (element) element.parentNode.removeChild(element);

		var svg = d3
			.select('body')
			.append('svg')
			.attr('width', width)
			.attr('height', height)
			.attr('id', this.id);

		var force = d3.layout
			.force()
			.gravity(0.05)
			.distance(100)
			.charge(-100)
			.size([width, height]);

		let connections = [];
		this.connections.forEach((conn) => {
			connections.push({
				source: this.getNode(conn.fromNode.number),
				target: this.getNode(conn.toNode.number),
				weight: conn.weight,
				enabled: conn.enabled,
			});
		});

		let nodes = [];
		this.nodes.forEach((originalNode) => {
			let node = originalNode.clone();
			if (node.layer == 0) {
				node.fixed = true;
				node.y = height - height * 0.2;
				node.x = (width / this.inputs) * node.number + width / this.inputs / 2;
			}

			if (node.output) {
				node.fixed = true;
				node.y = height * 0.2;
				node.x =
					(width / this.outputs) * (node.number - this.inputs) +
					width / this.outputs / 2;
			}

			nodes.push(node);
		});

		force.nodes(nodes).links(connections).start();

		var link = svg
			.selectAll('.link')
			.data(connections)
			.enter()
			.append('line')
			.attr('class', 'link')
			.style('stroke-width', function (d) {
				return d.enabled
					? d.weight > 0
						? 0.3 + d.weight
						: 0.3 + d.weight * -1
					: 0;
			})
			.style('stroke', function (d) {
				return d.weight > 0 ? '#0f0' : '#f00';
			});

		var node = svg
			.selectAll('.node')
			.data(nodes)
			.enter()
			.append('g')
			.attr('class', 'node')
			.call(force.drag);

		node
			.append('circle')
			.attr('r', '5')
			.attr('fill', function (d) {
				return d.layer == 0 ? '#00f' : d.output ? '#f00' : '#000';
			});

		node
			.append('text')
			.attr('dx', 12)
			.attr('dy', '.35em')
			.text(function (d) {
				return (
					d.number +
					(d.layer > 0
						? '(' + activationsNames[d.activationFunction] + ')'
						: null)
				);
			});

		force.on('tick', function () {
			link
				.attr('x1', function (d) {
					return d.source.x;
				})
				.attr('y1', function (d) {
					return d.source.y;
				})
				.attr('x2', function (d) {
					return d.target.x;
				})
				.attr('y2', function (d) {
					return d.target.y;
				});

			node.attr('transform', function (d) {
				return 'translate(' + d.x + ',' + d.y + ')';
			});
		});

		var element = document.getElementById(this.id);
		document.getElementById(container).append(element);
	}
}
