#include <stdio.h>
#include <mpi.h>
#include <malloc.h>

#define MAX_SIZE 20
#define SEND_TAG 997
#define RET_TAG  998

void create_data(int *data, int size) {
  int i;
  for(i = 0; i < size; i++) {
    data[i] = i + 1;
  }
}

int process_data(int *data, int start, int end) {
  int i;
  int sum = 0;
  for(i = start; i < end ; i++) {
    sum += data[i];
  }
  return sum;
}

void send_data(int *data, int size, int nproc) {
  int id, ierr;
  int seg_size = size/nproc;
  int start = seg_size;
  int end = 2 * seg_size;

  for(id = 1; id < nproc - 1; id++) {
    ierr = MPI_Send(&seg_size, 1 , MPI_INT, id, SEND_TAG, MPI_COMM_WORLD);
    ierr = MPI_Send(&data[start], seg_size, MPI_INT, id, SEND_TAG, MPI_COMM_WORLD);
    start = end;
    end = end + seg_size;
  }

  if (nproc > 1) {
    seg_size = size - start;
    ierr = MPI_Send(&seg_size, 1 , MPI_INT, id, SEND_TAG, MPI_COMM_WORLD);
    ierr = MPI_Send(&data[start], seg_size, MPI_INT, id, SEND_TAG, MPI_COMM_WORLD);
  }
}

int main(int argc, char **argv) {
  MPI_Status status;
  int i, ierr, myid, nproc, size = MAX_SIZE;
  int data[MAX_SIZE], *part_data;
  int end, root_process = 0, seg_size, part_sum, sum = 0;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &nproc);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);

  if (0 == myid) {
    create_data(data, size);
    send_data(data, size, nproc);
    end = nproc == 1 ? size : size/nproc;
    sum = process_data(data, 0, end);
    for(i = 1; i < nproc; i++) {
      ierr = MPI_Recv(&part_sum, 1, MPI_INT, MPI_ANY_SOURCE, RET_TAG, MPI_COMM_WORLD, &status);
      sum += part_sum;
    }
    printf("Sum of numbers = %d\n", sum);
  }
  else {
    ierr = MPI_Recv(&seg_size, 1, MPI_INT, root_process, SEND_TAG, MPI_COMM_WORLD, &status);
    part_data = (int *) malloc(sizeof(int) * seg_size);
    ierr = MPI_Recv(part_data, seg_size, MPI_INT, root_process,
                     SEND_TAG, MPI_COMM_WORLD, &status);
    part_sum = process_data(part_data, 0, seg_size);
    ierr = MPI_Send(&part_sum, 1, MPI_INT, root_process, RET_TAG, MPI_COMM_WORLD);
    free(part_data);
  }

  ierr = MPI_Finalize();
  return 0;
}
