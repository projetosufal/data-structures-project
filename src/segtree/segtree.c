#include <stdio.h>
#include <stdlib.h>
#define DEBUG if(0)

int
min_tree(int a, int b)
{
  if(a == 0)
    return b;
  if(b == 0)
    return a;
  
  if(a > b)
    return b;
  else
    return a;
}

int
max_tree(int a, int b)
{
  if(a == 0)
    return b;
  if(b == 0)
    return a;
  
  if(a < b)
    return b;
  else
    return a;
}

int
sum_tree(int a, int b)
{
  return a + b;
}

void
updatetree(int (*f)(int l_num, int r_num), int *tree,
	   int node, int min, int max, int l, int r, int val)
{
  int mid;
  if(min > max || min > r || max < l)
    return ;
  
  if(min == max)
    {
      tree[node] = val;
      return;
    }
  
  mid = (min+max)/2;
      
  updatetree((*f), tree, 2*node + 1, min , mid, l, r, val);
  updatetree((*f), tree, 2*node + 2, mid + 1 , max, l, r, val);
  
  tree[node] = (*f)(tree[2*node +1], tree[2*node + 2]);
}

int
query(int (*f)(int l_num, int r_num), int *tree,
      int node, int min, int max, int l, int r)
{
  if(r < min || max < l)
    return 0;
  
  if(l <= min && max <= r)
    return tree[node];
  
  int mid, l_bipod, r_bipod;  
  mid = (min+max)/2;
  
  l_bipod = query((*f), tree, 2*node + 1, min , mid, l , r);
  r_bipod = query((*f), tree, 2*node + 2, mid + 1 , max, l, r);
  return((*f)(l_bipod, r_bipod));
}

void
buildtree(int (*f)(int l_num, int r_num), int *v, int *tree,
	  int *t_size, int node, int min, int max)
{
  int mid;
  
  if(min == max)
    tree[node] = v[min];

  else
    {
      mid = (min+max)/2;
      
      buildtree((*f), v, tree, t_size, 2*node + 1, min , mid);
      buildtree((*f), v, tree, t_size, 2*node + 2, mid + 1 , max);
      tree[node] = (*f)(tree[2*node +1], tree[2*node + 2]);
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
  
  buildtree(sum_tree, v, tree, &t_size, 0, 0, v_size-1);
  //  buildtree(min_tree, v, tree, &t_size, 0, 0, v_size-1);
  //  buildtree(max_tree, v, tree, &t_size, 0, 0, v_size-1);
  
  for(i=0;i<t_size;++i)
    {
      printf("A[%d] = %d\n", i, tree[i]);
    }
  
  int left, right, n_v;
  
  DEBUG
    {
      scanf("%d %d", &left, &right);
      printf("%d\n",query(sum_tree, tree, 0, 0, v_size-1, left, right));
      //      printf("%d\n",query(min_tree, tree, 0, 0, v_size-1, left, right));
      //      printf("%d\n",query(max_tree, tree, 0, 0, v_size-1, left, right));
    }
  
  scanf("%d %d %d", &left, &right, &n_v);

  updatetree(sum_tree, tree, 0, 0, v_size-1, left, right, n_v);

  for(i=0;i<t_size;++i)
    {
      printf("A[%d] = %d\n", i, tree[i]);
    }
}
