#ifndef MONTEKARLOMETHOD_H_
#define MONTEKARLOMETHOD_H_

#include <iostream>
#include <string>
#include <map>
#include <random>
#include <chrono>

#include "Lattice.h"
#include "Node.h"
#include "Stack.h"

class VariativeValue{
protected:
	double start, end, step;
	size_t current_step{0}, step_amount;
public:
	VariativeValue(const double &start, const double &end, const double &step=0.1):
		start{start}, end{end},
		step{((end-start)*step>=0.0)?step:-step}, step_amount{static_cast<size_t>((end-start)/this->step)}{}
	VariativeValue& operator++(){
		++current_step;
		return *this;
	}
	const size_t& get_index()const{return current_step;}
	void reset(){current_step=0;}
	bool is_finished()const{return current_step >= step_amount;}
	double get_value()const{return start + current_step*step;}
};


class ProbabilityFunction{
protected:
	std::vector<double> values;
	inline static constexpr int max_value=8, energy_interval=2;
public:
    ProbabilityFunction(const double &T){
    	for(int i=-max_value; i<=max_value; i+=energy_interval)
    		values.push_back(std::exp(-(static_cast<double>(i)/T)));
    }
    double operator()(const short &dE)const{return values.at((dE+max_value)/energy_interval);}
};

class MonteKarloMethod{
protected:
	//std::chrono::nanoseconds(std::chrono::system_clock::now()).count()
	std::mt19937 generator{std::random_device{}()};
	std::uniform_real_distribution<> real_distribution{0, 1};
	std::map<std::string, std::string> parameters;
	Lattice lattice;
public:
	LatticeDataCollection collection;
	MonteKarloMethod(const std::map<std::string, std::string> &parameters):
		parameters(parameters), lattice{std::stoull(parameters.at("L"))}{
			if(parameters.count("T_start") == 0){
				this->parameters.emplace("T_start", "3.0");}
			if(parameters.count("T_end") == 0){
				this->parameters.emplace("T_end", "2.0");}
			if(parameters.count("T_step") == 0){
				this->parameters.emplace("T_step", "0.1");}
			if(parameters.count("steps_amount") == 0){
				this->parameters.emplace("steps_amount", "1000");}
			if(parameters.count("initial_steps_amount") == 0){
				this->parameters.emplace("initial_steps_amount", "10000");}
			if(parameters.count("averaging_steps_amount") == 0){
				this->parameters.emplace("averaging_steps_amount", "10000");}
			if(parameters.count("config_amount") == 0){
				this->parameters.emplace("config_amount", "1");}
			if(parameters.count("wolf_clusters_per_step") == 0){
				this->parameters.emplace("wolf_clusters_per_step", "10");}
		}
	void start(const std::string &algorithm_name="Metropolis", const bool &is_equilibrium=false){
		if(is_equilibrium){
			if(algorithm_name.compare("Metropolis")==0){
				start_metropolis();
			}else if(algorithm_name.compare("Wolf")==0){
				start_wolf();
			}
		}else{
			if(algorithm_name.compare("Metropolis")==0){
				start_metropolis_eq();
			}else if(algorithm_name.compare("Wolf")==0){
				start_wolf_eq();
			}
		}
	}
private:
	void start_metropolis(){
		std::map<std::string, double> data;
		size_t  mc_steps = std::stoull(parameters.at("steps_amount")),
				metropolis_depth = lattice.size*lattice.size,
				config_amount = std::stoull(parameters.at("config_amount"));
		for(VariativeValue T{std::stod(parameters.at("T_start")),
							std::stod(parameters.at("T_end")),
							std::stod(parameters.at("T_step"))};
			!T.is_finished(); ++T){
			ProbabilityFunction probability{T.get_value()};
			for(size_t config_index=0; config_index<config_amount; ++config_index){
				lattice.reset();
				for(size_t step=0; step<mc_steps; ++step){
					monte_carlo_metropolis_step(probability, metropolis_depth);
					lattice.get_data(data);
					collection.add(T.get_index(), data);
					data.clear();
				}
			}
		}
	}
	void start_metropolis_eq(){
		std::map<std::string, double> data;
		size_t  equilibrium_steps = std::stoull(parameters.at("steps_amount")),
				initial_equilibrium_steps = std::stoull(parameters.at("initial_steps_amount")),
				averaging_steps = std::stoull(parameters.at("averaging_steps_amount")),
				metropolis_depth = lattice.size*lattice.size,
				config_amount = std::stoull(parameters.at("config_amount"));
		for(VariativeValue T{std::stod(parameters.at("T_start")),
							std::stod(parameters.at("T_end")),
							std::stod(parameters.at("T_step"))};
			!T.is_finished(); ++T){
			ProbabilityFunction probability{T.get_value()};
			for(size_t config_index=0; config_index<config_amount; ++config_index){
				lattice.reset();
				const size_t eq_steps{(T.get_index()==0)?initial_equilibrium_steps:equilibrium_steps},
							mc_steps{eq_steps+averaging_steps};
				for(size_t step=0; step<mc_steps; ++step){
					monte_carlo_metropolis_step(probability, metropolis_depth);
					if(step > eq_steps){
						lattice.get_data(data);
						collection.add(T.get_index(), data);
						data.clear();
					}
				}
			}
		}
	}
	void start_wolf(){
		std::map<std::string, double> data;
		size_t  mc_steps = std::stoull(parameters.at("steps_amount")),
				wolf_depth = lattice.size*lattice.size,
				cluster_amount = std::stoull(parameters.at("wolf_clusters_per_step")),
				config_amount = std::stoull(parameters.at("config_amount"));
		Stack<Node> nodes_stack{wolf_depth};
		for(VariativeValue T{std::stod(parameters.at("T_start")),
							std::stod(parameters.at("T_end")),
							std::stod(parameters.at("T_step"))};
			!T.is_finished(); ++T){
			const double probability_value{1.0 - std::exp(2.0/T.get_value())};
			for(size_t config_index=0; config_index<config_amount; ++config_index){
				lattice.reset();
				for(size_t step=0; step<mc_steps; ++step){
					for(size_t cluster_num=0; cluster_num<cluster_amount; ++cluster_num)
						monte_carlo_wolf_step(probability_value, nodes_stack);
					lattice.get_data(data);
					collection.add(T.get_index(), data);
					data.clear();
				}
			}
		}
	}
	void start_wolf_eq(){
		std::map<std::string, double> data;
		size_t  equilibrium_steps = std::stoull(parameters.at("steps_amount")),
				initial_equilibrium_steps = std::stoull(parameters.at("initial_steps_amount")),
				averaging_steps = std::stoull(parameters.at("averaging_steps_amount")),
				wolf_depth = lattice.size*lattice.size,
				cluster_amount = std::stoull(parameters.at("wolf_clusters_per_step")),
				config_amount = std::stoull(parameters.at("config_amount"));
		Stack<Node> nodes_stack{wolf_depth};
		for(VariativeValue T{std::stod(parameters.at("T_start")),
							std::stod(parameters.at("T_end")),
							std::stod(parameters.at("T_step"))};
			!T.is_finished(); ++T){
			const double probability_value{1.0 - std::exp(2.0/T.get_value())};
			for(size_t config_index=0; config_index<config_amount; ++config_index){
				lattice.reset();
				const size_t eq_steps{(T.get_index()==0)?initial_equilibrium_steps:equilibrium_steps},
							mc_steps{eq_steps+averaging_steps};
				for(size_t step=0; step<mc_steps; ++step){
					for(size_t cluster_num=0; cluster_num<cluster_amount; ++cluster_num)
						monte_carlo_wolf_step(probability_value, nodes_stack);
					if(step > eq_steps){
						lattice.get_data(data);
						collection.add(T.get_index(), data);
						data.clear();
					}
				}
			}
		}
	}
	void monte_carlo_metropolis_step(const ProbabilityFunction &probability, const double& metropolis_depth){
		for(size_t step=0; step<metropolis_depth; ++step){
			Node node{{static_cast<size_t>((lattice.size-1)*real_distribution(generator))},
					{static_cast<size_t>((lattice.size-1)*real_distribution(generator))}};
			const short dE = 2*lattice.calculate_neighbors(node);
			if(real_distribution(generator) <= probability(dE)){
				lattice[node] = -lattice(node);
			}
		}
	}
	void monte_carlo_wolf_step(const double &probability_value, Stack<Node> &nodes_stack){
		nodes_stack.clear();
		Node init_node{{static_cast<size_t>((lattice.size-1)*real_distribution(generator))},
						{static_cast<size_t>((lattice.size-1)*real_distribution(generator))}};
		const short initial_spin{lattice(init_node)};
		lattice[init_node] = -initial_spin;
		nodes_stack.push(init_node);
		while(!nodes_stack.empty()){
			const Node node{nodes_stack.pop()};
			wolf_elementary_flip(initial_spin, probability_value, lattice.top(node), nodes_stack);
			wolf_elementary_flip(initial_spin, probability_value, lattice.bottom(node), nodes_stack);
			wolf_elementary_flip(initial_spin, probability_value, lattice.left(node), nodes_stack);
			wolf_elementary_flip(initial_spin, probability_value, lattice.right(node), nodes_stack);
		}
	}
	void wolf_elementary_flip(const short &initial_spin, const double &probability_value,
								const Node &node, Stack<Node> &nodes_stack){
		if (lattice(node)*initial_spin==1 and probability_value>=real_distribution(generator)) {
			lattice[node] = -lattice(node);
			nodes_stack.push(node);
		}
	}
};


#endif /* MONTEKARLOMETHOD_H_ */
