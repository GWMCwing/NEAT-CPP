// interface between the network and the game
class Player {
	constructor(id) {
		this.brain = new Genome(genomeInputsN, genomeOutputsN, id);
		this.fitness;

		this.score = 1;
		this.lifespan = 0;
		this.dead = false;
		this.decisions = []; // current output values
		this.vision = []; // current input values
	}

	clone() {
		let clone = new Player();
		clone.brain = this.brain.clone();
		return clone;
	}

	crossover(parent) {
		let child = new Player();
		if (parent.fitness < this.fitness) {
			child.brain = this.brain.crossover(parent.brain);
		} else {
			child.brain = parent.brain.crossover(this.brain);
		}
		child.brain.mutate();
		return child;
	}

	// game stuff
	look() {
		// this.vision = [points[this.lifespan].x, points[this.lifespan].y];
		// this.correctVal = points[this.lifespan].type;

		//! input from the environment
		this.vision = [];
	}

	think() {
		this.decisions = this.brain.feedForward(this.vision);
	}

	move() {
		// let maxIndex = 0;
		// for (let i = 0; i < this.decisions.length; i++) {
		// 	if (this.decisions[i] > this.decisions[maxIndex]) {
		// 		maxIndex = i;
		// 	}
		// }
		// this.val = this.decisions[maxIndex] >= 0 ? 1 : 0;
		// TODO call the player movement
	}

	update() {
		// if (this.correctVal == this.val) {
		// 	this.score++;
		// }
		// this.lifespan++;
		// if (this.lifespan >= points.length) {
		// 	this.dead = true;
		// }
		//TODO update game stuff
		// update dead or alive, score, etc.
	}

	calculateFitness() {
		this.fitness = this.score;
		this.fitness /= this.brain.calculateWeight();
	}

	show() {
		// show the player
	}
}
