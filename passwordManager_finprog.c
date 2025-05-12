#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Account{
    char username[50];
    char password[50];
    struct Account* next;
}Account;

int hashFunction(char* key, int size){
    int hash = 0;
    while(*key){
        hash = (hash + *key) % size;
        key++;
    }
    return hash;
}

void createAccount(Account** hashTable, int size, char* username, char* password){
    int index = hashFunction(username, size);
    Account* newAcc = (Account*)malloc(sizeof(Account));
    strcpy(newAcc->username, username);
    strcpy(newAcc->password, password);
    newAcc->next = hashTable[index];
    hashTable[index] = newAcc;
    printf("Akun berhasil dibuat.\n");
}

void readAccounts(Account** hashTable, int size){
    printf("\n--- Daftar Akun ---\n");
    for(int i = 0; i < size; i++){
        Account* temp = hashTable[i];
        while(temp){
            printf("Username: %s\n", temp->username);
            printf("Password: %s\n", temp->password);
            temp = temp->next;
        }
    }
}

Account* searchAccount(Account** hashTable, int size, char* username){
    int index = hashFunction(username, size);
    Account* temp = hashTable[index];
    while(temp){
        if(strcmp(temp->username, username) == 0)
            return temp;
        temp = temp->next;
    }
    return NULL;
}

void updateAccount(Account** hashTable, int size, char* username, char* newPassword){
    Account* acc = searchAccount(hashTable, size, username);
    if(acc){
        strcpy(acc->password, newPassword);
        printf("Password berhasil diperbarui.\n");
    }else{
        printf("Akun tidak ditemukan.\n");
    }
}

void deleteAccount(Account** hashTable, int size, char* username){
    int index = hashFunction(username, size);
    Account* temp = hashTable[index];
    Account* prev = NULL;
    while(temp){
        if(strcmp(temp->username, username) == 0){
            if(prev)
                prev->next = temp->next;
            else
                hashTable[index] = temp->next;
            free(temp);
            printf("Akun berhasil dihapus.\n");
            return;
        }
        prev = temp;
        temp = temp->next;
    }
    printf("Akun tidak ditemukan.\n");
}

int main(){
    const int tableSize = 100;
    Account** hashTable = (Account**)calloc(tableSize, sizeof(Account*));
    int choice;
    char username[50];
    char password[50];

    while(1){
        printf("PASSWORD MANAGER\n");
        printf("1.Buat akun\n");
        printf("2.Tampilkan semua akun\n");
        printf("3.Cari akun\n");
        printf("4.Ubah password\n");
        printf("5.Hapus akun\n");
        printf("6.Keluar\n");
        printf("Pilihan: ");
        scanf("%d", &choice);
        getchar();

        switch(choice){
            case 1:
                printf("Masukkan username: ");
                fgets(username, sizeof(username), stdin);
                username[strcspn(username, "\n")] = 0;
                printf("Masukkan password: ");
                fgets(password, sizeof(password), stdin);
                password[strcspn(password, "\n")] = 0;
                createAccount(hashTable, tableSize, username, password);
                break;
            case 2:{
                int isEmpty = 1;
                for (int i = 0; i < tableSize; i++) {
                    if (hashTable[i] != NULL) {
                        isEmpty = 0;
                        break;
                    }
                }
                if(isEmpty){
                    printf("Belum ada akun yang tersimpan.\n");
                }else{
                    readAccounts(hashTable, tableSize);
                }
                break;
            }
            case 3:{
                printf("Masukkan username yang dicari: ");
                fgets(username, sizeof(username), stdin);
                username[strcspn(username, "\n")] = 0;
                Account* found = searchAccount(hashTable, tableSize, username);
                if(found){
                    printf("Ditemukan\n");
                    printf("Username: %s\n", found->username);
                    printf("Password: %s\n", found->password);
                }else{
                    printf("Akun tidak ditemukan\n");
                }
                break;
            }
            case 4:
                printf("Masukkan username: ");
                fgets(username, sizeof(username), stdin);
                username[strcspn(username, "\n")] = 0;
                printf("Masukkan password baru: ");
                fgets(password, sizeof(password), stdin);
                password[strcspn(password, "\n")] = 0;
                updateAccount(hashTable, tableSize, username, password);
                break;
            case 5:
                printf("Masukkan username: ");
                fgets(username, sizeof(username), stdin);
                username[strcspn(username, "\n")] = 0;
                deleteAccount(hashTable, tableSize, username);
                break;
            case 6:
                printf("Terima kasih telah menggunakan Password Manager.\n");
                for(int i = 0; i < tableSize; i++){
                    Account* temp = hashTable[i];
                    while(temp){
                        Account* toFree = temp;
                        temp = temp->next;
                        free(toFree);
                    }
                }
                free(hashTable);
                return 0;
            default:
                printf("Pilihan tidak valid.\n");
        }
    }
}
