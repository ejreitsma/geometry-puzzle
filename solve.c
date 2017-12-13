#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

typedef struct variable_struct {
  int min;
  int max;
} variable;

typedef struct equation_struct {
  int sum;
  int index[4];
} equation;
  
void read_config(const char* fname,
		 equation eqs[]);

void read_sizes(const char* fname);

void init_variables(variable vars[], int count);

void print_vars(variable var[], int count);

void print_solution(variable var[], int count);

int update_ranges(variable vars[], equation eqs[], int eq_count);

int is_solution(variable vars[], int count);

void generate_solutions(variable vars[], equation eqs[], int eq_count, int index);

int var_count = 0;
int eq_count = 0;

int main(int argc, char** argv) {
  variable* vars;
  equation* eqs;

  if (argc < 2) {
    return 1;
  }
  
  read_sizes(argv[1]);

  printf("equations: %d variables: %d\r\n", eq_count, var_count);
  
  eqs = malloc(eq_count * sizeof(equation));
  vars = malloc(var_count * sizeof(variable));
  
  read_config(argv[1], eqs);

  init_variables(vars, var_count);

  update_ranges(vars, eqs, eq_count);
  
  generate_solutions(vars, eqs, eq_count, 1);
    
  return 0;
}

void init_variables(variable vars[], int count)
{
  for (int i = 1; i < count; i++) {
    vars[i].min = 1;
    vars[i].max = 26;
  }
  vars[0].min = 0;
  vars[0].max = 0;
}

void read_config(const char* fname,
		 equation eqs[])
{
  FILE* fp = fopen(fname, "r");

  int sum, i1, i2, i3, i4;

  int index = 0;

  while (fscanf(fp, "%d %d %d %d %d",
		&sum,
		&i1,
		&i2,
		&i3,
		&i4
  ) != EOF)
    {
      eqs[index].sum = sum;
      eqs[index].index[0] = i1;
      eqs[index].index[1] = i2;
      eqs[index].index[2] = i3;
      eqs[index].index[3] = i4;
      index++;
    }
  fclose(fp);
}

void read_sizes(const char* fname)
{
  FILE* fp = fopen(fname, "r");

  eq_count = 0;
  var_count = 0;
  
  int sum, i1, i2, i3, i4;

  int index = 0;

  while (fscanf(fp, "%d %d %d %d %d",
		&sum,
		&i1,
		&i2,
		&i3,
		&i4
  ) != EOF)
    {
      if (i1 > var_count)
	{
	  var_count = i1;
	}
      if (i2 > var_count)
	{
	  var_count = i2;
	}
      if (i3 > var_count)
	{
	  var_count = i3;
	}
      if (i4 > var_count)
	{
	  var_count = i4;
	}
      index++;
      eq_count++;
    }
  var_count += 1;
  fclose(fp);
}

void generate_solutions(variable vars[], equation eqs[], int eq_count, int index)
{
  variable* new_vars;

  new_vars = malloc(var_count * sizeof(variable));

  for (int i = vars[index].min; i <= vars[index].max; i++)
    {
      memcpy(new_vars, vars, var_count * sizeof(variable));
      
      new_vars[index].min = i;
      new_vars[index].max = i;
      
      if (update_ranges(new_vars, eqs, eq_count))
	{
	  if (is_solution(new_vars, var_count))
	    {
	      print_solution(new_vars, var_count);
	    }
	  else
	    {
	      generate_solutions(new_vars, eqs, eq_count, index + 1);
	    }
	}

    }
  free(new_vars);

}

int update_ranges(variable vars[], equation eqs[], int eq_count)
{
  int update_count = 0;

  int updated = 1;

  while (updated) {
    updated = 0;
    update_count++;
    for (int e = 0; e < eq_count; e++) {
      int min_sum = 0;
      int max_sum = 0;
      for (int v = 0; v < 4; v++)
	{
	  min_sum += vars[eqs[e].index[v]].min;
	  max_sum += vars[eqs[e].index[v]].max;
	}
      for (int v = 0; v < 4; v++)
	{	 
	  int min =
	    eqs[e].sum - (max_sum - vars[eqs[e].index[v]].max);
	  if (min > vars[eqs[e].index[v]].min)
	    {
	      updated = 1;
	      vars[eqs[e].index[v]].min = min;
	    }

	  int max =
	    eqs[e].sum - (min_sum - vars[eqs[e].index[v]].min);
	  if (max < vars[eqs[e].index[v]].max)
	    {
	      updated = 1;
	      vars[eqs[e].index[v]].max = max;
	    }
	  if (vars[eqs[e].index[v]].min > vars[eqs[e].index[v]].max)
	    {
	      return 0;
	    }
	}
    }
  }

  return update_count;
}

void print_vars(variable var[], int count)
{
  for (int i = 0; i < count; i++)
    {
      printf("%d: %d to %d\r\n", i, var[i].min, var[i].max);
    }
}

void print_solution(variable var[], int count)
{
  for (int i = 1; i < count; i++)
    {
      printf("%c", 'a' + var[i].min - 1);
    }
  printf("\r\n");
}

int is_solution(variable vars[], int count)
{
  for (int i = 0; i < count; i++)
    {
      if (vars[i].min != vars[i].max)
	{
	  return 0;
	}
    }
  return 1;
}
