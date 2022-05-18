// TODO refactor it as a class
var activationsNames = ['Sigmoid', 'Identity', 'Step', 'Tanh', 'ReLU']; // used in svg drawing
// Box-Muller transform
// https://stackoverflow.com/questions/25582882/javascript-math-random-normal-distribution-gaussian-bell-curve
function randNum_bm(min, max, skew) {
	let u = 0,
		v = 0;
	while (u === 0) u = Math.random(); //Converting [0,1) to (0,1)
	while (v === 0) v = Math.random();
	let num = Math.sqrt(-2.0 * Math.log(u)) * Math.cos(2.0 * Math.PI * v);

	num = num / 10.0 + 0.5; // Translate to 0 -> 1
	if (num > 1 || num < 0) num = randNum_bm(min, max, skew);
	// resample between 0 and 1 if out of range
	else {
		num = Math.pow(num, skew); // Skew
		num *= max - min; // Stretch to fill range
		num += min; // offset to min
	}
	return num;
}
/**
 *
 */
class Node {
	constructor(number, layer, isOutput) {
		this.number = number;
		this.layer = layer;
		this.activationFunction = Math.floor(Math.random() * 5); // index of activation function
		this.bias = Math.random() * 2 - 1;
		this.output = isOutput || false;

		this.inputSum = 0;
		this.outputValue = 0;
		this.outputConnections = [];
	}

	// pass down the calculated output value
	engage() {
		if (this.layer != 0) {
			this.outputValue = this.activation(this.inputSum + this.bias);
		}
		for (let i = 0; i < this.outputConnections.length; i++) {
			const conn = this.outputConnections[i];
			if (conn.enabled) {
				// do not pass if disabled
				conn.toNode.inputSum += conn.weight * this.outputValue;
			}
		}
	}

	// randomly mutate the bias of this node
	mutateBias() {
		if (Math.random() < 0.05) {
			this.bias = Math.random() * 2 - 1;
		} else {
			//! replace non-standard gaussian function with box-muller transform
			this.bias += randNum_bm(-1, 1, 1) / 50;
		}
	}

	// randomly choose a new activation function
	mutateActivation() {
		this.activationFunction = Math.floor(Math.random() * 5);
	}

	// check if two nodes are connected
	isConnected(node) {
		// same layer cannot be connected
		if (node.layer == this.layer) {
			return false;
		}
		// check parameter node connections
		if (node.layer < this.layer) {
			for (let i = 0; i < node.outputConnections.length; i++) {
				// is node connected to this node
				if (node.outputConnections[i].toNode == this) {
					return true;
				}
			}
		} else {
			// check this node connections
			for (let i = 0; i < this.outputConnections.length; i++) {
				// is this node connected to node
				if (this.outputConnections[i].toNode == node) {
					return true;
				}
			}
		}
		return false;
	}

	// return copy of this node
	clone() {
		let node = new Node(this.number, this.layer, this.output);
		node.bias = this.bias;
		node.activationFunction = this.activationFunction;
		return node;
	}

	activation(x) {
		switch (this.activationFunction) {
			case 0: // sigmoid
				return 1 / (1 + Math.pow(Math.E, -4.9 * x));
			case 1: // identity
				return x;
			case 2: // step
				return x > 0 ? 1 : 0;
			case 3: // tanh
				return Math.tanh(x);
			case 4: // ReLU
				return x > 0 ? x : 0;
			default:
				return 1 / (1 + Math.pow(Math.E, -4.9 * x));
		}
	}
}
