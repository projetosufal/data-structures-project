#include <stdio.h>
#include <stdlib.h>

int
sum_leaf(int *v, int i, int proc)
{
  return v[i];
}

int
min_max_leaf(int *v, int i, int proc)
{
  return i;
}

int
min_tree(int l, int r)
{
  if(l > r)
    return r;
  else
    return l;
}

int
max_tree(int l, int r)
{
  if(l < r)
    return r;
  else
    return l;
}
  
int
sum(int l, int r)
{
  return l + r;
}

void
buildtree(int (*f)(int l, int r), int (*leaf)(int *v, int i, int proc),
	  int *v, int *tree, int *t_size, int key, int min, int max)
{
  int mid;
  
  if(min == max)
    //    tree[key] = (*leaf)(v, min, key);
    tree[key] = max;
  else
    {
      mid = (min+max)/2;
      
      buildtree((*f), (*leaf), v, tree, t_size, 2*key+1, min , mid);
      buildtree((*f), (*leaf), v, tree, t_size, 2*key+2, mid+1 , max);
      tree[key] = (*f)(tree[2*key +1], tree[2*key + 2]);
    }
}

int
main(void)
{
  int *v, *tree, v_size, t_size, i;

  scanf("%d", &v_size);
  t_size = (2*v_size) + 1;
  
  v = malloc(sizeof(int) * v_size);
  tree = malloc(sizeof(int) * t_size);


  for(i=0;i<v_size; ++i)
    {
      scanf("%d", &v[i]);
    }
  
  //  buildtree(sum, sum_leaf, v, tree, &t_size, 0, 0, v_size-1);
  buildtree(min_tree, min_max_leaf, v, tree, &t_size, 0, 0, v_size-1);
  
  for(i=0;i<t_size;++i)
    {
      printf("A[%d] = %d\n", i, tree[i]);
    }
}
