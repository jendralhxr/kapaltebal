#define POPULATION 20
#define GENERATION_MAX 50

int main(void)
{
	int i, ite, df;
	double prev_tt[MMM];
	clock_t start, end, lap1, lap2;


//-------------------------------INPUT MATERIAL PRICE (START)---------------------------------//
//	for (i = 0; i < num_mate; i++)
	for (i = 0; i < beam_mat_1st; i++)
	{
		tt[i] = 1.0;

		if (mp[4][i] == 188) mate_cost[i] = 60;
		else if (mp[4][i] == 252) mate_cost[i] = 80;
		else mate_cost[i] = 90;
	}
	//-------------------------------INPUT MATERIAL PRICE (END)---------------------------------//

	// coba sini
	double a1, a2, a3, a4, dv;
	double AS, AS_bend, AS_shear, AS_buckle;
	int seed;

	char *gene[POPULATION];
	char *parent[2];

	double cost[POPULATION]; // cost untuk semua individu 
	double cost_best, cost_parent[2]; // best= terbaik dari optimasi thickness FEM, parent= terbaik dari tiap generasi
	
	for (i = 0; i < POPULATION; i++) gene[i] = (char*)malloc(sizeof(char) * beam_mat_1st);  //gen sebanyak beam_mat_1st utk tiap individu 
	parent[0] = (char*)malloc(sizeof(char) * beam_mat_1st);  // gen kedua induk terpilih
	
	parent[1] = (char*)malloc(sizeof(char) * beam_mat_1st);

	// initialize population
	for (int ret = 0; ret < POPULATION; ret++) {
		for (i = 0; i < beam_mat_1st; i++) {
			gene[ret][i] = rand() % 3 + '0'; // 0, 1, 2    //gen awal dibangkitkan secara acak
		}
	}

	// initialize individual 1 from input file
	// fill here

	for (int generation=0; generation < GENERATION_MAX; generation++) {
		cost_parent[0] = 6e6; cost_parent[1] = 6e6;
		printf("\n\n ========== Generation %d ==========", generation);
		// coba cek tiap generasi
		printf("The strongest individual each generation : ");
		for (int i = 0; i < beam_mat_1st; i++) printf("%c", gene[0][i]);
		printf("\n");

		
		for (int ret = 0; ret < POPULATION; ret++) { // for each individual
			printf("\n\n ========== Invididual %d ==========", ret);
			// update thickness, checkpoint
		//	AS = mp[4][i];
		//	AS_bend = AS * 0.8;
		//	AS_shear = AS * 0.4;
		//	AS_buckle = AS * 0.8;
		//	a1 = sig_mises_max[i] / AS;
		//	a2 = sqrt(sig_bending_max[i] / AS_bend);
		//	a3 = sig_shear_max[i] / AS_shear;
		//	a4 = sig_buckling_max[i] / AS_buckle;
		//	dv = a1;
		//	if (a2 > dv) dv = a2;
		//	if (a3 > dv) dv = a3;
		//	if (a4 > dv) dv = a4;
		//	tt[i] *= dv;
		//	if (i >= 0 && i < beam_mat_1st && tt[i] < (6.0 / mp[3][i]))  tt[i] = 6.0 / mp[3][i];
		//	if (i >= beam_mat_1st && i < num_mate)                        tt[i] = 1.0;

			cost_best = 6e6; 
			for (ite = 0; ite < MAX_ITE; ite++) {
				df = 0;
				printf("\n\n ========== Iteration %d ==========", ite);
				//fprintf(str_file, "\n\n ---------- Iteration %d ----------", ite);
				printf("Start");
				FEM(ite);

				for (i = 0; i < num_mate; i++)
					prev_tt[i] = tt[i];

				update_designVariable1(); // Step 1 - Low grade
				printf("End");

				for (i = 0; i < num_mate; i++) {
					if ((fabs(tt[i] - prev_tt[i]) / tt[i]) * 100 > DECISION) df++; //convergency, checkpoint
				}
				if (df == 0) {
					printf("\n\n ////////// Convergence completed //////////");
					printf("Start df");
					FEM(ite);
					printf("End df");
					fprintf(str_file, "\n\n,////////// Optimal Solution //////////");
					print_str(ite);
					fprintf(volume_file, "%lf,%lf,%.0f\n ", weight, price, mp[4][i]);
					}
					break;
				}
				printf("\nMinimum price, individual %d, interation %d：　%lf", ret, ite, price);
				lap2 = clock(); // CPU lap時間計測 - k.gonde
				printf("\nProcessing time(%d time)　：　%d [ms]", ite, lap2 - lap1);
				lap1 = lap2;
			
			// penghitungan harga tiap individu
			for (i = 0; i < beam_mat_1st; i++) {
				//weight += vol_plt_m[i] * mp[0][i];
				switch (gene[ret][i]) {
				case('0'): mp[4][i] = 188; mate_cost[i] = 60; break;
				case('1'): mp[4][i] = 252; mate_cost[i] = 80; break;
				case('2'): mp[4][i] = 284; mate_cost[i] = 90; break;
				}
			cost[ret] += vol_plt_m[i] * mp[0][i] * mate_cost[i];
			}
			printf("\nCost individual %d, interation %d：　%lf", ret, ite, cost[ret]); // harga dengan coba-coba ganti material
			
			// fittness test for each individual to find two best individuals
			if (cost[ret] < cost_parent) {
				cost_parent[1] = cost_parent[0];
				cost_parent[0] = cost_ret[ret];
				memcpy(parent[1], parent[0], sizeof(char) * beam_mat_1st); // second best individual
				memcpy(parent[0], gene[ret], sizeof(char) * beam_mat_1st); // newly found second best individual
				}
			}

	
	// two parents continue to next generation
	memcpy(gene[0], parent[0], sizeof(char)* beam_mat_1st);
	memcpy(gene[1], parent[1], sizeof(char)* beam_mat_1st);
	
	// crossver between the two parents and mutation
	for (int ret = 2; ret < POPULATION; ret++) {
		retry:
		for (i = 0; i < beam_mat_1st; i++) {
			seed = rand() % 1000;
			if (seed < 462) gene[ret][i] = parent[0][i];
			else if (seed > 512) gene[ret][i] = parent[1][i];
			else gene[ret][i] = rand() % 3 + '0';
		}
		// check for duplicate
		for (int cand=0; cand < ret; cand++){
			if (strcmp(gene[ret], gene[cand])==0) goto retry; 
			}
	}

	// harga terbarik tiap generasi
	fprintf(volume_file, "%lf,%lf\n", weight, cost_parent[0]);
	//for (int i = 0; i < beam_mat_1st; i++) fprintf(volume_file, "%c", gene[0][i]);
	//print_str(ite);
}


// assign fittest individual as solution
for (i = 0; i < beam_mat_1st; i++) {
	switch (gene[0][i]) {
	case('0'): mp[4][i] = 188; mate_cost[i] = 60; break;
	case('1'): mp[4][i] = 252; mate_cost[i] = 80; break;
	case('2'): mp[4][i] = 284; mate_cost[i] = 90; break;
	}
	weight += vol_plt_m[i] * mp[0][i];
	price += vol_plt_m[i] * mp[0][i] * mate_cost[i]; // final price, checkpoint
}

fprintf(str_file, "\n\n,////////// Optimal Solution //////////");
fprintf(volume_file, "price %lf,%lf\n", weight, price);
fprintf(volume_file, "cost %lf,%lf\n", weight, cost_parent[0]);
printf("The strongest individual: "); for (int i = 0; i < beam_mat_1st; i++) printf("%c", gene[0][i]);
for (int i = 0; i < beam_mat_1st; i++) fprintf(volume_file, "%c", gene[0][i]);
	

	return (0);
}
