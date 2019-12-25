#define POPULATION 20
#define GENERATION_MAX 50

void update_designVariableGA(){ // random cross-over
	double a1, a2, a3, a4, dv;
	double AS, AS_bend, AS_shear, AS_buckle;
	
	char gene[POPULATION][num_mate];
	char parent[2][num_mate];
	double cost[2]={6e6,6e6}, cost_temp, weight;
	int seed;
	
	// initialize population
	for (int ret=0; ret<POPULATION; ret++){
		for (i=1; i<num_mate; i++){
			gene[ret][i]= rand()%3+'0'; // 0, 1, 2
			}
		}
	
	// initialize individual 1 from input file
	// fill here
	
	for (int generation; generation<GENERATION_MAX; generation++){
		// find two fittest individuals, cost calculation;
		for (int ret=0; ret<POPULATION; ret++){ // for each individual
			
			// update thickness, checkpoint
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
		
			// FEM(ite) checkpoint
			for (ite = 0; ite < MAX_ITE; ite++){
				df = 0; 
			//	fprintf(str_file, "\n\n ---------- Iteration %d ----------", ite);
				FEM(ite);
				for (i = 0; i < num_mate; i++) prev_tt[i] = tt[i];
				if (ite != 0) {
					price_ratio = price / prev_price;
					weight_ratio = weight / prev_weight;
					}
				if (ite == 0) {
					prev_price = price;
					prev_weight = weight;
					}
				// fprintf(volume_file, "%lf,%lf,%lf\n", weight_ratio, price_ratio, weight_ratio + price_ratio);
				for (i = 0; i < num_mate; i++) {
					if ((fabs(tt[i] - prev_tt[i]) / tt[i]) * 100 > DECISION) df++; //convergency, checkpoint
					}
				if (df == 0) {
					FEM(ite); 
					if (ite != 0) {
						price_ratio = price / prev_price;
						weight_ratio = weight / prev_weight;
					}
				//	fprintf(volume_file, "%lf,%lf,%lf\n", weight_ratio, price_ratio, weight_ratio + price_ratio);
					break;
					}
			}
				
			// cost calculation for each individual
			cost_temp= 0.0;
			for (i=0; i<num_mate; i++){ 
				weight += vol_plt_m[i] * mp[0][i];
				switch (gene[ret][i]){
					case('0'): mate_cost[i]= 80; break;
					case('1'): mate_cost[i]= 85; break;
					case('2'): mate_cost[i]= 90; break;
					}
				cost_temp += vol_plt_m[i] * mp[0][i] * mate_cost[i];
				}
				
				// fittness test
			if (cost_temp<cost[0]){
				cost[1]= cost[0];
				cost[0]= cost_temp;
				memcpy (parent[1], parent[0]); // second best individual
				memcpy (parent[0], gene[ret]); // newly found second best individual
				}
 			}
		}
		
		// crossver between the two parents (0.462 each) and mutation (0.075)
		for (int ret=1; ret<POPULATION; ret++){
			for (i=0; i<num_mate; i++){
				seed= rand()%1000;
				if (seed<462) gene[ret][i]= parent[0][i];
				else if (seed>537) gene[ret][i]= parent[1][i]; 
				else gene[ret][i]= rand()%3+'0';
				}
			}
		
		}
	
	// assign fittest individual as solution
	for (i=0; i < num_mate; i++){
		switch (gene[0][i]){
			case('0'): mp[4][i]= 188; mate_cost[i]= 80; break;
			case('1'): mp[4][i]= 252; mate_cost[i]= 85; break;
			case('2'): mp[4][i]= 284; mate_cost[i]= 90; break;
			}
		weight += vol_plt_m[i] * mp[0][i];
		price += vol_plt_m[i] * mp[0][i] * mate_cost[i]; // final price, checkpoint
		}
		
	}

