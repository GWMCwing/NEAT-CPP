let genomeInputsN = 2;
let genomeOutputN = 1;
let showBest = true;

class Population {
	constructor(size) {
		this.population = [];
		this.bestPlayer;
		this.bestFitness = 0;

		this.generation = 0;
		this.matingPool = [];

		for (let i = 0; i < size; i++) {
			this.population.push(new Player(i));
			this.population[i].brain.generateNetwork();
			this.population[i].brain.mutate();
		}
	}

	updateAlive() {
		for (let i = 0; i < this.population.length; i++) {
			const player = this.population[i];
			if (!player.dead) {
				player.look();
				player.think();
				player.move();
				player.update();
			}
		}

		if (showBest && this.bestPlayer && !this.bestPlayer.dead) {
			this.bestPlayer.look();
			this.bestPlayer.think();
			this.bestPlayer.move();
			this.bestPlayer.update();
			this.bestPlayer.show();
		}
	}

	done() {
		for (let i = 0; i < this.population.length; i++) {
			if (!this.population[i].dead) {
				return false;
			}
		}
		return true;
	}

	naturalSelection() {
		this.calculateFitness();

		let averageSum = this.getAverageScore();
		console.log(averageSum);
		let children = [];

		this.fillMatingPool();
		for (let i = 0; i < this.population.length; i++) {
			let parent1 = this.selectPlayer();
			let parent2 = this.selectPlayer();
			if (parent1.fitness > parent2.fitness) {
				children.push(parent1.crossover(parent2));
			} else {
				children.push(parent2.crossover(parent1));
			}
		}

		this.population.splice(0, this.population.length);
		this.population = children.slice(0);
		this.generation++;
		for (let i = 0; i < this.population.length; i++) {
			this.population[i].brain.generateNetwork();
		}

		console.log('Generation: ' + this.generation);

		this.bestPlayer.lifespan = 0;
		this.bestPlayer.dead = false;
		this.bestPlayer.score = 1;
	}

	calculateFitness() {
		let currentMax = 0;
		for (let i = 0; i < this.population.length; i++) {
			const player = this.population[i];
			player.calculateFitness();
			if (player.fitness > this.bestFitness) {
				this.bestFitness = player.fitness;
				this.bestPlayer = player.clone();
				this.bestPlayer.brain.id = 'BestGenome';
				this.bestPlayer.brain.draw();
			}

			if (player.fitness > currentMax) {
				currentMax = player.fitness;
			}
		}

		// normalize
		for (let i = 0; i < this.population.length; i++) {
			this.population[i].fitness /= currentMax;
		}
	}

	fillMatingPool() {
		this.matingPool.splice(0, this.matingPool.length);
		for (let i = 0; i < this.population.length; i++) {
			let n = this.population[i].fitness * 100;
			for (let j = 0; j < n; j++) {
				// ? why index only
				this.matingPool.push(i);
			}
		}
	}

	selectPlayer() {
		const rand = Math.floor(Math.random() * this.matingPool.length);
		return this.population[this.matingPool[rand]];
	}

	getAverageScore() {
		let averageSum = 0;
		for (let i = 0; i < this.population.length; i++) {
			averageSum += this.population[i].score;
		}
		return averageSum / this.population.length;
	}
}
