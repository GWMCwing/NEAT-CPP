class Connection {
	constructor(from, to, weight) {
		this.fromNode = from;
		this.toNode = to;
		this.weight = weight;
		this.enabled = true;
	}
	mutateWeight() {
		const rand = Math.random();
		if (rand < 0.05) {
			this.weight = Math.random() * 2 - 1;
		} else {
			this.weight += randNum_bm(-1, 1, 1) / 50;
		}
	}
	clone() {
		let clone = new Connection(this.fromNode, this.toNode, this.weight);
		clone.enabled = this.enabled;
		return clone;
	}
	getInnovationNumber() {
		return (
			(1 / 2) *
				(this.fromNode.number + this.toNode.number) *
				(this.fromNode.number + this.toNode.number + 1) +
			this.toNode.number
		);
	}
}
