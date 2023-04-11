#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_B 5 //Max Words to be searched
#define SHOW_HALF_RESULT 1 //If 2 words searched then show results of 1 words in similar results
#define SEARCH_WORD_ONLY 1 //Search only words not half of string.
#define SHOW_PR 1 //Debug Show priorty

// Details of code
// Time Complexity - (total news).(no of words to be searched).(lenght of news).(lenght of searched string)
// Storing on frequency array, (total news)^2 changed into (total news)
// Space Complexity - 10.(total news max)
// Considering for half results 10.(no of words to be searched).(total news max)

struct engine_data
{
    int priority;
    char search_feed[150];
};

typedef struct engine_data ED;
ED search_list[300];

// Array to store Freq.
int arr[10][300];
int k[10]={0};

// Spliting String
int b=0;
char w[MAX_B][300];

int global_count = 0;


int charcasecmp(char a,char b) {
    if(a>='A'&&a<='Z') {
        a=a-'A'+'a';
    }
    if(b>='A'&&b<='Z') {
        b=b-'A'+'a';
    }
    if(a==b) return 1;
    return 0;
}

void split(char a[300]) {
    int i=0,t=0;
    char s[300];
    s[0]='\0';
    while(a[i]!='\0') {
        if(a[i]==' ') {
            if(b==MAX_B) {
               printf("Only %d Words supported",MAX_B);
               b=0;
               return;
            }
            strcpy(w[b],s);
            b++;
            t=0;
            s[t]='\0';
        } else {
            s[t]=a[i];
            s[t+1]='\0';
            t++;
        }
        i++;
    }
    if(b==MAX_B) {
       printf("\n\tOnly %d Words supported",MAX_B);
       b=0;
       return;
    }
    strcpy(w[b],s);
    b++;
}


/**
Function Name: welcome
Input Params: NIL
Return Type: void
Description: Prints a welcome message
**/
void welcome()
{
    int  i = 0;
    for(i=0;i<80;i++)
        printf("*");

    printf("\n\n\t\t\t\tIII SEARCH ENGINE\n\n\n");

    for(i=0;i<80;i++)
        printf("*");

    printf("\n\n");

}

/**
Function Name: load_from_file
Input Params: NILL
Return Type: void
Description: loads all the data available with search engine into appropriate data structure
**/
void load_from_file()
{
    FILE *fp;

    /// Open the file in read mode
    fp = fopen("SearchIndex.txt", "r");

    /// Check if the file was successfully opened
    if (fp == NULL)
    {
        printf("File Not found\n");
        return;
    }

    /// Until the end of the file, read all the file data
    while(!feof(fp))
    {
        fscanf(fp,"%d\t%[^\n]s", &search_list[global_count].priority, search_list[global_count].search_feed);
        printf("%d\t%s\n", search_list[global_count].priority, search_list[global_count].search_feed);
        //Sort by frequency
        arr[search_list[global_count].priority-1][k[search_list[global_count].priority-1]]=global_count;
        k[search_list[global_count].priority-1]++;
        global_count++;
    }

    fclose(fp);
}

void search(char a[]) {
    // Store half results
    int halfarr[MAX_B][10][300];
    int halfin[MAX_B][10]={0};
    int m,n,u,f=0,p,l,d=0,hf=-1;
    n=strlen(a);
    split(a);
    if(b==0) return;
    //for(int i=0;i<b;i++) {
        //printf("%s\n",w[i]);
    //}
    for(int i=9;i>=0;i--) {
        for(int j=0;j<k[i];j++) {
            l=strlen(search_list[arr[i][j]].search_feed);
            int tt=0;
            for(int t=0;t<b;t++) {
                n=strlen(w[t]);
                for(int z=0;z<l-n+1;z++) {
                    u=0;
                    if (SEARCH_WORD_ONLY) {
                        if(z>0) {
                            //printf("\n%c\n",search_list[arr[i][j]].search_feed[z+u]);
                            if((search_list[arr[i][j]].search_feed[z+u-1]>='a'&&search_list[arr[i][j]].search_feed[z+u-1]<='z')||(search_list[arr[i][j]].search_feed[z+u-1]>='A'&&search_list[arr[i][j]].search_feed[z+u-1]<='Z')) {
                                continue;
                            }
                        }
                    }
                    while(u<n&&charcasecmp(w[t][u],search_list[arr[i][j]].search_feed[z+u])==1) {
                        u++;
                    }
                    //printf("%d",u);
                    if(SEARCH_WORD_ONLY&&u==n&&(search_list[arr[i][j]].search_feed[z+u]<='a'||search_list[arr[i][j]].search_feed[z+u]>='z')) {
                        tt++;
                        break;
                    } else if(!SEARCH_WORD_ONLY&&u==n) {
                        //printf("%s found in %d",w[t],z);
                        tt++;
                        break;
                    }
                }
            }
            if(tt==b) {
                f=1;
                if(SHOW_PR) {
                    printf("\n\n\t (%d) %s",search_list[arr[i][j]].priority,search_list[arr[i][j]].search_feed);
                } else {
                    printf("\n\n\t%s",search_list[arr[i][j]].search_feed);
                }
                printf("\n\tPress 1 to load more result other number to exit\n\t:");
                scanf("%d",&p);
                if(p==1) {
                    f=0;
                } else {
                    d=1;
                }
            } else if(tt>0&&SHOW_HALF_RESULT) {
                halfarr[tt][i][halfin[tt][i]]=arr[i][j];
                halfin[tt][i]++;
                hf=1;
            }
        }
        if(d) break;
    }
    if(f==0) {
        printf("\n\tResult not found\n");
        d=0;
        int bk=0;
        if(hf==1) {
            printf("\n\tShowing similar result");
            for(int t=b-1;t>=0;t--) {
                for(int i=9;i>=0;i--) {
                    for(int j=0;j<halfin[t][i];j++) {
                        if(SHOW_PR) {
                            printf("\n\n\t (%d) %s",search_list[halfarr[t][i][j]].priority,search_list[halfarr[t][i][j]].search_feed);
                        } else {
                            printf("\n\n\t%s",search_list[halfarr[t][i][j]].search_feed);
                        }
                        d=0;
                        printf("\n\tPress 1 to load more result other number to exit\n\t:");
                        scanf("%d",&p);
                        if(p==1) {
                            d=1;
                        } else {
                            bk=1;
                            break;
                        }
                    }
                    if(bk==1) break;
                }
                if(bk==1) break;
            }
        }
        if(d==1) printf("\n\tResult not found");
    }
}

int main()
{
    welcome();
    load_from_file();
    int s=0;
    char a[100];
    printf("\n\tSearch: ");
    scanf("%[^\n]s",a);
    do {
        search(a);
        printf("\n\n");
        for(int i=0;i<80;i++)
            printf("*");
        printf("\n\n\tDo you want to Search Again [1/0]?");
        scanf("%d",&s);
        if(s==1) {
            printf("\n\tSearch: ");
            scanf("%*c%[^\n]s",a);
        }
        b=0;
    } while(s==1);
    // TODO

    return 0;
}

