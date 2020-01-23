#define POPULATION 20
int main(void)
#define GENERATION_MAX 50

int main(void)
{
	int i, ite, df;
	double prev_tt[MMM];  // 前ステップの設計変数 - k.gonde
	clock_t start, end, lap1, lap2;

	// coba sini
	double a1, a2, a3, a4, dv;
	double AS, AS_bend, AS_shear, AS_buckle;

	char *gene[POPULATION];
	char *parent[2];

	for (i=0; i < POPULATION; i++) gene[i] = (char*) malloc(sizeof(char) * num_mate);
	parent[0] = (char*) malloc(sizeof(char) * num_mate);
	parent[1] = (char*) malloc(sizeof(char) * num_mate);


	double cost[2] = { 6e6,6e6 }, cost_temp, weight;
	int seed;
	
	// initialize population
	for (int ret = 0; ret < POPULATION; ret++) {
		for (i = 1; i < num_mate; i++) {
			gene[ret][i] = rand() % 3 + '0'; // 0, 1, 2
		}
	}

	// initialize individual 1 from input file
	// fill here

	for (int generation=0; generation < GENERATION_MAX; generation++) {
		printf("\n\n ========== Generation %d ==========", generation);
		// coba cek tiap generasi
		printf("The strongest individual each generation : ");
		for (int i = 0; i < num_mate; i++) printf("%c", gene[0][i]);
		printf("\n");

		
		// find two fittest individuals, cost calculation;
		for (int ret = 0; ret < POPULATION; ret++) { // for each individual
			printf("\n\n ========== Invididual %d ==========", ret);
			// update thickness, checkpoint
			AS = mp[4][i];
			AS_bend = AS * 0.8;
			AS_shear = AS * 0.4;
			AS_buckle = AS * 0.8;
			a1 = sig_mises_max[i] / AS;
			a2 = sqrt(sig_bending_max[i] / AS_bend);
			a3 = sig_shear_max[i] / AS_shear;
			a4 = sig_buckling_max[i] / AS_buckle;
			dv = a1;
			if (a2 > dv) dv = a2;
			if (a3 > dv) dv = a3;
			if (a4 > dv) dv = a4;
			tt[i] *= dv;
			if (i >= 0 && i < beam_mat_1st && tt[i] < (6.0 / mp[3][i]))  tt[i] = 6.0 / mp[3][i];
			if (i >= beam_mat_1st && i < num_mate)                        tt[i] = 1.0;

			// FEM(ite) checkpoint
			for (ite = 0; ite < MAX_ITE; ite++) {
				df = 0;
				printf("\n\n ========== Iteration %d ==========", ite);
				//fprintf(str_file, "\n\n ---------- Iteration %d ----------", ite);
				printf("Start");
				FEM(ite);

				if (ite == 1) price_min[0] = price;
				else if (ite != 0 && price < price_min[0]) price_min[0] = price;

				for (i = 0; i < num_mate; i++)
					prev_tt[i] = tt[i];
				if (ite != 0) {
					price_ratio = price / prev_price;
					weight_ratio = weight / prev_weight;
				}
				if (ite == 0) {
					prev_price = price = 0;
					prev_weight = weight = 0;
				}
			//	fprintf(volume_file, "%lf,%lf,%lf\n", weight_ratio, price_ratio, weight_ratio + price_ratio);

				update_designVariable1(); // Step 1 - Low grade
				
				printf("End");
			//	for (i = 0; i < num_mate; i++) {
			//		prev_tt[i] = tt[i];
				
			//	}
			//	if (ite != 0) {
			//		price_ratio = price / prev_price;
			//		weight_ratio = weight / prev_weight;
			//	}
			//	if (ite == 0) {
			//		price = 0;
			//		weight = 0;
			//		prev_price = price;
			//		prev_weight = weight;
			//	}
				//fprintf(volume_file, "%lf,%lf,%lf\n", weight_ratio, price_ratio, weight_ratio + price_ratio);

				for (i = 0; i < num_mate; i++) {
					if ((fabs(tt[i] - prev_tt[i]) / tt[i]) * 100 > DECISION) df++; //convergency, checkpoint
				}
				if (df == 0) {
					printf("\n\n ////////// Convergence completed //////////");
					fprintf(str_file, "\n\n,////////// Optimal Solution //////////");
					printf("Start df");
					FEM(ite);
					printf("End df");
					if (ite != 0) {
						price_ratio = price / prev_price;
						weight_ratio = weight / prev_weight;
					}
				//		fprintf(volume_file, "%lf,%lf,%lf\n", weight_ratio, price_ratio, weight_ratio + price_ratio);
						fprintf(volume_file, "%lf,%lf\n", weight, price);
					break;
				}
				lap2 = clock(); // CPU lap時間計測 - k.gonde
				printf("\nProcessing time(%d time)　：　%d [ms]", ite, lap2 - lap1);
				lap1 = lap2;
			}

			// cost calculation for each individual
			cost_temp = 0.0;
			weight = 0;
			for (i = 0; i < beam_mat_1st; i++) {
				weight += vol_plt_m[i] * mp[0][i];
				switch (gene[ret][i]) {
				case('0'): mp[4][i] = 188; mate_cost[i] = 60; break;
				case('1'): mp[4][i] = 252; mate_cost[i] = 80; break;
				case('2'): mp[4][i] = 284; mate_cost[i] = 90; break;
				}
				cost_temp += vol_plt_m[i] * mp[0][i] * mate_cost[i];
			}

			// fittness test
			if (cost_temp < cost[0]) {
				cost[1] = cost[0];
				cost[0] = cost_temp;
				memcpy(parent[1], parent[0], sizeof(char) * beam_mat_1st); // second best individual
				memcpy(parent[0], gene[ret], sizeof(char) * beam_mat_1st); // newly found second best individual
			}
		}
	

	// crossver between the two parents (0.462 each) and mutation (0.075)
	for (int ret = 2; ret < POPULATION; ret++) {
		for (i = 0; i < beam_mat_1st; i++) {
			seed = rand() % 1000;
			if (seed < 462) gene[ret][i] = parent[0][i];
			else if (seed > 537) gene[ret][i] = parent[1][i];
			else gene[ret][i] = rand() % 3 + '0';
		}
	}
	// two parents continue to next generation
	memcpy(gene[0], parent[0], sizeof(char) * beam_mat_1st);
	memcpy(gene[1], parent[1], sizeof(char) * beam_mat_1st);


fprintf(volume_file, "%lf,%lf,%lf\n", weight, price, cost_temp);
for (int i = 0; i < beam_mat_1st; i++) fprintf(volume_file, "%c", gene[0][i]);
print_str(ite);
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
fprintf(volume_file, "%lf,%lf\n", weight, price);
printf("The strongest individual : ");
for (int i = 0; i < beam_mat_1st; i++) printf("%c", gene[0][i]);
for (int i = 0; i < beam_mat_1st; i++) fprintf(volume_file, "%c", gene[0][i]);
	

	end = clock(); // CPU時間計測終了 - k.gonde
	printf("\nProcessing time (Total)　：　%d [ms]", end - start);
	fprintf(str_file, "\nProcessing time (Total)　：　%d [ms]", end - start);


	fclose(inp_file);
	fclose(out_file);
	fclose(str_file);
	fclose(volume_file);

	printf("\n\nEnd of calculation　 Hit「Enter key」\n");
	getchar();

	return (0);
}
