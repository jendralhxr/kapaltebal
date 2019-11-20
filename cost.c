#define POPULATION 20
#defien GENERATION_MAX 50

void update_designVariableGA(){ // random cross-over
	double a1, a2, a3, a4, dv;
	double AS, AS_bend, AS_shear, AS_buckle;
	
	char gene[POPULATION][num_mate];
	char parent[2][num_mate];
	float cost[2], cost_temp;
	int seed;
	
	// initialize population
	for (int ret=0; ret<POPULATION; ret++){
		for (i=0; i<num_mate; i++){
			gene[ret][i]= rand()%3+'0'; // 0, 1, 2
			}
		}
	
	for (int generation; generation<GENERATION_MAX; generation++){
		// find fittest two individual, cost calculation;
		for (int ret=0; ret<POPULATION; ret++){
			for (i=0; i<num_mate; i++){
				cost_temp += // how to calculate cost
				
				if (cost_temp<cost[0]){
					//cost[0] <-- cost_temp
					//cost[1] <-- cost[0]
					//parent[0] <-- ret
					//parent[1] <-- parent[0]
					}
 				}
			}
		
		// crossver between the two parents (0.462 each) and mutation (0.075)
		for (int ret=0; ret<POPULATION; ret++){
			for (i=0; i<num_mate; i++){
				seed= rand()%1000;
				if (seed<462) gene[ret][i]= parent[0][i];
				else if (seed>537) gene[ret][i]= parent[1][i]; 
				else gene[ret][i]= rand()%3+'0';
				}
			}
		}
	
	// assign fittest individual
	for (i=0; i < num_mate; i++){
		switch (parent[0][i]){
			case('0'): mp[4][i] = 188; mate_cost[i] = 80; break;
			case('1'): mp[4][i] = 252; mate_cost[i] = 85; break;
			case('2'): mp[4][i] = 284; mate_cost[i] = 90; break;
			}
		
		AS = mp[4][i];
		AS_bend = AS*0.8;
		AS_shear = AS*0.4;
		AS_buckle = AS*0.8;
	
		a1 = sig_mises_max[i] / AS;              
		a2 = sqrt(sig_bending_max[i] / AS_bend); 
		a3 = sig_shear_max[i] / AS_shear;      
		a4 = sig_buckling_max[i] / AS_buckle; 
	
		dv = a1;
		if (a2 > dv) dv = a2;
		if (a3 > dv) dv = a3;
		if (a4 > dv) dv = a4;
	
		tt[i] *= dv; 
	
		if (i >= 0 && i < beam_mat_1st  && tt[i] < (6.0 / mp[3][i]))  tt[i] = 6.0 / mp[3][i];  
		if (i >= beam_mat_1st && i < num_mate)                        tt[i] = 1.0;
		}
	}

