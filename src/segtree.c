#include <stdio.h>
#include <stdlib.h>

void
buildtree(int *A, int *tree,
	  int key, int min, int max)
{
  int mid;
  
  if(min == max)
    tree[key] = A[min];
  else
    {
      mid = (min+max)/2;
      buildtree(A, tree, 2*key+1, min , mid);
      buildtree(A, tree, 2*key+2, mid+1 , max);
      tree[key] = tree[2*key+1] + tree[2 *key+2];
    }
}

int
main(void)
{
  int i;
  int max = 6;
  int A[max];

  for(i=0;i<max; ++i)
    {
      scanf("%d", &A[i]);
    }
    
  int tree[max*2+1];

  for(i=0;i<max*2;++i)
    {
      tree[i] = -1;
    }
    
  buildtree(A, tree, 0, 0, max-1);
  
  for(i=0;i<max*2+1;++i)
    {
      printf("A[%d] = %d\n", i, tree[i]);
    }
}
