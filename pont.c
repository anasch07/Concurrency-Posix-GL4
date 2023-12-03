#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>

pthread_mutex_t mutex ;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int poids_dispo = 15;
int attente_camion = 0;
int attente_voiture = 0;


void* voiture(void* arg){
    pthread_mutex_lock(&mutex);
    // printf("Voiture %d arrive\n",*((int*)(&arg)));
    while((poids_dispo == 0) || (poids_dispo == 15 && attente_camion != 0)){
        attente_voiture++;
        // printf("  Voiture %d Attends\n",*((int*)(&arg)));
        pthread_cond_wait(&cond, &mutex);
        attente_voiture--;
    }
    printf("Voiture %d dans le pont\n",*((int*)(&arg)));
    poids_dispo-=5;
    pthread_mutex_unlock(&mutex);

    sleep(2);

    pthread_mutex_lock(&mutex);
    printf("Voiture %d sort du pont\n",*((int*)(&arg)));
    poids_dispo+=5;
    printf("V : %d  C : %d \n",attente_voiture, attente_camion);
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);
}

void* camion(void* arg){
    pthread_mutex_lock(&mutex);
    // printf("Camion %d arrive\n",*((int*)(&arg)));
    while(poids_dispo < 15){
        attente_camion++;
        // printf("  Camion %d Attends\n",*((int*)(&arg)));
        pthread_cond_wait(&cond, &mutex);
        attente_camion--;
    }
    printf("Camion %d dans le pont\n",*((int*)(&arg)));
    poids_dispo-=15;
    pthread_mutex_unlock(&mutex);
    sleep(2);

    pthread_mutex_lock(&mutex);
    printf("Camion %d sort du pont\n",*((int*)(&arg)));
    poids_dispo+=15;
    printf("V : %d  C : %d \n",attente_voiture, attente_camion);
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);
}


int main(int argc, char* argv[]){

	pthread_t t1;
	pthread_t t2;
	pthread_t t3;
	pthread_t t4;
	pthread_t t5;
	pthread_t t6;
	pthread_t t7;

	pthread_mutex_init(&mutex, NULL);

	pthread_create(&t1, NULL, voiture,(void*) 1);
	pthread_create(&t2, NULL, voiture,(void*) 2);

	pthread_create(&t4, NULL, camion,(void*) 4);

	pthread_create(&t3, NULL, voiture,(void*) 3);
	pthread_create(&t5, NULL, voiture,(void*) 5);

	pthread_create(&t6, NULL, camion,(void*) 6);
    
	pthread_create(&t7, NULL, voiture,(void*) 7);

	pthread_join(t1,NULL);
	pthread_join(t2,NULL);
	pthread_join(t3,NULL);
	pthread_join(t4,NULL);
	pthread_join(t5,NULL);
	pthread_join(t6,NULL);
	pthread_join(t7,NULL);

}