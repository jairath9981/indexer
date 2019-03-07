#include<stdio.h>
#include<string.h>
#include <ctype.h>
#include<stdlib.h>
#include<conio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<time.h>

#define directory "C:\\Users\\HP\\Desktop\\crawlerHelp"
#define HASH_SIZE 100

int array_sizes;


struct main_tags
{
	char tags[6];
	int count;	
};


struct link
{
	char *file_name;
	struct link *next;
	struct  main_tags *down;
};


struct node
{
	char *word;
	struct node *right;
	struct node *left;
	struct link *head;
	struct link *tail;
};


struct hash
{
	struct node *head;
};


struct main_tags *create_main_tags()
{
	
	struct main_tags *new_node;
	new_node = (struct  main_tags*)malloc(5*sizeof(struct main_tags)); // title,h1,h2,h3,h4.........
	strcpy(new_node[0].tags,"title");
	new_node[0].count = 0;
	strcpy(new_node[1].tags,"h1");
	new_node[1].count = 0;
	strcpy(new_node[2].tags,"h2");
	new_node[2].count = 0;
	strcpy(new_node[3].tags,"h3");
	new_node[3].count = 0;
	strcpy(new_node[4].tags,"h4");
	new_node[4].count = 0;
	return new_node;
}


struct link* create_link(char *file_path)
{
	int len = strlen(file_path);
	struct link *new_node = (struct link*)malloc(1*sizeof(struct link));
	new_node->file_name=(char *)malloc(len*sizeof(char));
	strcpy(new_node->file_name,file_path);
	new_node->next = NULL;
	new_node->down = create_main_tags();
	return new_node;	
}

struct node* create_node(char *word,char *file_path)
{
	int len = strlen(word);
	struct node *new_node = (struct node*)malloc(1*sizeof(struct node));
	new_node->word=(char *)malloc(len*sizeof(char));
	strcpy(new_node->word,word);
	new_node->left = NULL;
	new_node->right = NULL;
	new_node->head =  create_link(file_path);
	new_node->tail = new_node->head;	
	return new_node;
}


struct node* insert(struct node* node, char *word, int i,char *file_path) 
{ 
    if (node == NULL) return create_node(word,file_path); 
  
    // If key already exists in BST, icnrement count and return 
    if (strcmp(node->word,word)==0) 
    { 
        struct link *temp = (node->tail);
        if(strcmp(temp->file_name,file_path)==0)
        {
        	struct main_tags *tags = temp->down;
        	((*(tags+i)).count)++;
		}
  		else
  	   {
  	 		temp = create_link(file_path);
  	 		(node->tail)->next=temp;
  	 		node->tail=temp;
		}
        return node; 
    } 
  
    /* Otherwise, recur down the tree */
    if (strcmp(node->word,word)>0) 
        node->left  = insert(node->left,word,i,file_path); 
    else
        node->right = insert(node->right, word,i,file_path); 
  
    /* return the (unchanged) node pointer */
    return node; 
} 


int generateKey(char *word)
{
int sum=0,i;
for(i=0;word[i]!='\0';i++)
{
sum=sum+word[i];
}
return sum%HASH_SIZE;
}


void fill_hash_table(char *word , int key ,struct hash *h,int i,char *file_path)
{
	struct node *root = NULL;
	if(h[key].head==NULL)
	{
		root = insert(root,word,i,file_path);
		h[key].head = root;
	}
	else
	{
		root = h[key].head;
		insert(root,word,i,file_path);
	}
	
}


void make_file_name(char *file_path, int file_number)
{
	char str[10];
	sprintf(str,"%d",file_number);
	strcat(file_path,directory);
	strcat(file_path,"/file ");
	strcat(file_path,str);
	strcat(file_path,".txt ");
}


char *get_file_data(char *file_path)
{
	int file_size,i=0;
	char c;
	FILE *fp;
	fp=fopen(file_path,"r");
	struct stat st;     //variable which will count length of file..........
	stat(file_path,&st);    // temp.txt is the file where wget fetch the html
	file_size=st.st_size;
	array_sizes=file_size;
	char *file_data = (char*)malloc(file_size*sizeof(char));
	c=fgetc(fp);
	while(c!=EOF)
	{
		file_data[i]=c;
		i++;
		c=fgetc(fp);
	}
	fclose(fp);
	file_data [i]='\0';
	return file_data ;
}


void extract_usefull_tags_data(char *title,char *h1,char *h2, char *h3,char *h4,char *file_data)
{
	int len = array_sizes;
	int i,t=0,h=0,flag,k,p=0;
	int ft=0,fh1=0,fh2=0,fh3=0,fh4=0;
	char temp,temp2,tag[8],end_of_tag[1000];
	char define_title_tag[] = "<title";
	char define_h1_tag[] = "<h1" , define_h2_tag[] = "<h2";
	char define_h3_tag[] = "<h3" , define_h4_tag[] = "<h4";
	while(file_data[i]!='\0')
	{
		temp = tolower(file_data[i]);
		temp2=temp;
		t=0;
		h=0;
		if(temp==define_title_tag[t])  //<
		{
			k=i;
			p=0;
			while((temp==define_title_tag[t] || temp==' ' ) && (define_title_tag[t]!='\0'))
			{
				flag=0;
				if(temp==define_title_tag[t])
				{
					flag=1;
					//printf(" title= %c\n",temp);
					tag[p]=temp;
					p++;
					t++;
			    }
				if (flag==0 && temp!=' ')
				{
					break;
				}
				k++;
				temp = tolower(file_data[k]);
			}
			//printf(" end while  title %c\n",temp);
		}
		if(define_title_tag[t]=='\0')
		{
			i=k;
			//printf("woooo\n");
			tag[p]='\0';
		}
		if(temp2==define_h1_tag[h] && define_title_tag[t]!='\0') // <
		{
            temp=temp2;   
			k=i;
			p=0;
			while((temp==define_h1_tag[h] || temp==define_h2_tag[h] || temp==define_h3_tag[h] || temp==define_h4_tag[h] || temp==' ' ) && (define_h1_tag[h]!='\0' || define_h2_tag[h]!='\0' || define_h3_tag[h]!='\0' || define_h4_tag[h]!='\0'))
			{
				flag=0;
				if(temp==define_h1_tag[h] || temp==define_h2_tag[h] || temp==define_h3_tag[h] || temp==define_h4_tag[h])
				{
					flag=1;
					//printf("heading =  %c\n",temp);
					tag[p]=temp;
					p++;
					h++;
			    }
				if (flag==0 && temp!=' ')
				{
					break;
				}
				k++;
				temp = tolower(file_data[k]);
			}
			//printf(" end while  headng %c\n",temp);
			tag[p]='\0';
			i=k;
		}
		end_of_tag[0]=' ';
		if(strcmp(tag,define_title_tag)==0)	
		{
			while(file_data[i]!='>')
			{
				i++;
			}
			i++;
			while((strcmp(end_of_tag,"</title")!=0) && (file_data[i]!='-' || file_data[i+1]!='-' || file_data[i+2]!='>'))
			{
				temp = tolower(file_data[i]);
				if(temp=='<')
				{
					//printf("title = ");
					p=0;
					while(temp!='>')
					{ 
						//printf("%c",temp);
						end_of_tag[p]=temp;
						p++;
						i++;
						temp = tolower(file_data[i]);
					}
					i++;
					end_of_tag[p]='\0';
				}
				else
				{
					title[ft]=temp;
					ft++;
					i++;	
				}
			}
			title[ft]=' ';
			ft++;
			tag[0]=' ';
		}
		else if(strcmp(tag,define_h1_tag)==0)	
		{
			while(file_data[i]!='>')
			{
				i++;
			}
			i++;
			while((strcmp(end_of_tag,"</h1")!=0) && (file_data[i]!='-' || file_data[i+1]!='-' || file_data[i+2]!='>'))
			{
				temp = tolower(file_data[i]);
				if(temp=='<')
				{
					p=0;
					while(temp!='>')
					{
						end_of_tag[p]=temp;
						p++;
						i++;
						temp = tolower(file_data[i]);
					}
					i++;
					end_of_tag[p]='\0';
				}
				else
				{
					h1[fh1]=temp;
					fh1++;
					i++;	
				}
			}
			h1[fh1]=' ';
			fh1++;
			tag[0]=' ';
		}
		else if(strcmp(tag,define_h2_tag)==0)	
		{
			while(file_data[i]!='>')
			{
				i++;
			}
			i++;
			while((strcmp(end_of_tag,"</h2")!=0) && (file_data[i]!='-' || file_data[i+1]!='-' || file_data[i+2]!='>'))
			{
				temp = tolower(file_data[i]);
				if(temp=='<')
				{
					p=0;
					while(temp!='>')
					{
						end_of_tag[p]=temp;
						p++;
						i++;
						temp = tolower(file_data[i]);
					}
					i++;
					h2[fh2]=' ';
					fh2++;
					end_of_tag[p]='\0';
				}
				else
				{
					h2[fh2]=temp;
					//printf("%c",temp);
					fh2++;
					i++;	
				}
			}
			h2[fh2]=' ';
			fh2++;
			tag[0]=' ';

		}
		else if(strcmp(tag,define_h3_tag)==0)	
		{
			while(file_data[i]!='>')
			{
				i++;
			}
			i++;
			while((strcmp(end_of_tag,"</h3")!=0) && (file_data[i]!='-' || file_data[i+1]!='-' || file_data[i+2]!='>'))
			{
				temp = tolower(file_data[i]);
				if(temp=='<')
				{
					p=0;
					while(temp!='>')
					{
						end_of_tag[p]=temp;
						p++;
						i++;
						temp = tolower(file_data[i]);
					}
					i++;
					end_of_tag[p]='\0';
				}
				else
				{
					h3[fh3]=temp;
					fh3++;
					i++;	
				}
			}
			h3[fh3]=' ';
			fh3++;
			tag[0]=' ';

		}	
		else if(strcmp(tag,define_h4_tag)==0)	
		{
			while(file_data[i]!='>')
			{
				i++;
			}
			i++;
			while((strcmp(end_of_tag,"</h4")!=0) && (file_data[i]!='-' || file_data[i+1]!='-' || file_data[i+2]!='>'))
			{
				temp = tolower(file_data[i]);
				if(temp=='<')
				{
					p=0;
					while(temp!='>')
					{
						end_of_tag[p]=temp;
						p++;
						i++;
						temp = tolower(file_data[i]);
					}
					i++;
					end_of_tag[p]='\0';
				}
				else
				{
					h4[fh4]=temp;
					fh4++;
					i++;	
				}
			}
			h4[fh4]=' ';
			fh4++;
			tag[0]=' ';

		}
		i++;
	}
	title[ft]='\0';
	
	h1[fh1]='\0';  h2[fh2]='\0';
	h3[fh3]='\0';  h4[fh4]='\0';
}

void extract_title(char *title,struct hash *h,int j,char *file_path)
{
	int i=0,p=0,key;
	char word[1000];
	while(title[i]!='\0')
	{
		if(title[i]!=' ')
		{
			p=0;
			while(title[i]!=' ' && title[i]!='\0')
			{
				word[p]=title[i];
				p++;
				i++;	
			}
			word[p]='\0';
			if(strcmp(word,"\n")!=0)
			{
				printf("word = %s\n",word);
				//key = generateKey(word);
				//fill_hash_table(word,key,h,j,file_path);	
			}
		}
		else
		{
			i++;
		}
	}
}


int main()
{
	int i=0,file_number=1;
	struct hash h[HASH_SIZE] = { };
	char *file_data;
	char file_path[300];
	make_file_name(file_path,file_number);
	file_data = get_file_data(file_path);	
	char title[array_sizes] , h1[array_sizes] , h2[array_sizes], h3[array_sizes] ,h4[array_sizes];
	extract_usefull_tags_data(title,h1,h2,h3,h4,file_data);
	//printf("%s\n\n",h4);
	extract_title(title,h,0,file_path);
	extract_title(h1,h,1,file_path);
	extract_title(h2,h,2,file_path);
	extract_title(h3,h,3,file_path);
	extract_title(h4,h,4,file_path);
}
