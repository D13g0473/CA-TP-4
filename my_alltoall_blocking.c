#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Implementación de Alltoall usando comunicaciones punto a punto bloqueantes.
void My_Alltoall_blocking(const void* sendbuf, int sendcount, MPI_Datatype sendtype,
                          void* recvbuf, int recvcount, MPI_Datatype recvtype,
                          MPI_Comm comm) {
    int rank, size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    int sendtype_size, recvtype_size;
    MPI_Type_size(sendtype, &sendtype_size);
    MPI_Type_size(recvtype, &recvtype_size);

    // Copia local de los datos
    memcpy((char*)recvbuf + rank * recvcount * recvtype_size, 
           (const char*)sendbuf + rank * sendcount * sendtype_size, 
           sendcount * sendtype_size);

    // Estrategia para evitar deadlock: pares envían/reciben, impares reciben/envían
    if (rank % 2 == 0) {
        // Enviar a todos los demás
        for (int i = 0; i < size; ++i) {
            if (i != rank) {
                const void* sbuf = (const char*)sendbuf + i * sendcount * sendtype_size;
                MPI_Send(sbuf, sendcount, sendtype, i, 5, comm);
            }
        }
        // Recibir de todos los demás
        for (int i = 0; i < size; ++i) {
            if (i != rank) {
                void* rbuf = (char*)recvbuf + i * recvcount * recvtype_size;
                MPI_Recv(rbuf, recvcount, recvtype, i, 5, comm, MPI_STATUS_IGNORE);
            }
        }
    } else {
        // Recibir de todos los demás
        for (int i = 0; i < size; ++i) {
            if (i != rank) {
                void* rbuf = (char*)recvbuf + i * recvcount * recvtype_size;
                MPI_Recv(rbuf, recvcount, recvtype, i, 5, comm, MPI_STATUS_IGNORE);
            }
        }
        // Enviar a todos los demás
        for (int i = 0; i < size; ++i) {
            if (i != rank) {
                const void* sbuf = (const char*)sendbuf + i * sendcount * sendtype_size;
                MPI_Send(sbuf, sendcount, sendtype, i, 5, comm);
            }
        }
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int* send_buf = (int*)malloc(size * sizeof(int));
    int* recv_buf = (int*)malloc(size * sizeof(int));

    printf("Proceso %d, enviando:  [", rank);
    for (int i = 0; i < size; i++) {
        send_buf[i] = rank * 10 + i;
        printf("%d%s", send_buf[i], (i == size - 1) ? "" : ", ");
    }
    printf("]\n");

    My_Alltoall_blocking(send_buf, 1, MPI_INT, recv_buf, 1, MPI_INT, MPI_COMM_WORLD);

    printf("Proceso %d, recibido: [", rank);
    for (int i = 0; i < size; i++) {
        printf("%d%s", recv_buf[i], (i == size - 1) ? "" : ", ");
    }
    printf("]\n");

    free(send_buf);
    free(recv_buf);

    MPI_Finalize();
    return 0;
}
