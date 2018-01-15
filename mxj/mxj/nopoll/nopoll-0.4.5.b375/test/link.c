#include <iostream>  
#include <stdio.h>  
#include <malloc.h>  
#include <stdlib.h>    //exit(-1)  
using namespace std;  
  
typedef struct Node  
{  
    int data;//数据域   
    struct Node *pNext;//指针域   
}NODE,*PNODE;   
  
//函数声明 8种函数：    单链表的建立、遍历、判断是否为空、求长、插入、删除、排序、逆置；   
PNODE create_list();  
void traverse_list(PNODE pHead);  
bool is_empty(PNODE pHead);  
int length_list(PNODE pHead);  
bool insert_list(PNODE pHead,int val);  
bool delete_list(PNODE pHead,int Val);  

  
int main()  
{  
    int len;  
    PNODE pHead=NULL; 
	int Val = 23;
                           //动态内存才可以：在一个函数中分配，另一个函数也可以用   
    pHead=create_list();   
      //创建一个非循环单链表，并将该链表的头结点的地址赋给pHead  
  len=length_list(pHead); 
    cout<<"链表长度len="<<len<<endl; 
     
    traverse_list(pHead); 
    insert_list(pHead,33); 
    traverse_list(pHead); 
	delete_list(pHead,Val); 
	traverse_list(pHead);
     
    if(is_empty(pHead)) 
        cout<<"链表为空"<<endl; 
    else 
        cout<<"链表不空"<<endl;  
    /*     
    sort_list(pHead); 
    */
     
  
      
    return 0;   
}  
  
PNODE create_list()  
{  
    int len;//有效节点的个数   
    int i;  
    int val;//用来存放用户输入的结点的值   
      
    PNODE pHead=(PNODE)malloc(sizeof(NODE));  
    if(NULL==pHead)  
    {  
        cout<<"分配失败，程序终止！"<<endl;  
        exit(-1);   
    }  
      
    PNODE pTail=pHead;  
    pTail->pNext =NULL;  
      
    cout<<"请输入您需要生成的链表节点的个数：len=";  
    cin>>len;  
      
    for(i=0;i<len;++i)  
    {  
        cout<<"请输入第"<<i+1<<"个节点的值：";  
        cin>>val;  
           
        PNODE pNew=(PNODE)malloc(sizeof(NODE));  
        if(NULL==pNew)  
        {  
            cout<<"分配失败，程序终止！"<<endl;  
            exit(-1);   
        }  
          
        pNew->data=val;  
        pTail->pNext=pNew;  
        pNew->pNext = NULL;  
        pTail=pNew;  
     }   
       
     return pHead;  
  
}   
   
void traverse_list(PNODE pHead)  
{  
    PNODE p=pHead->pNext;  
    while(NULL!=p)  
    {  
        cout<<p->data<<" ";  
        p=p->pNext;  
    }  
    cout<<endl;  
      
    return;  //告诉别人函数执行完毕   
}  
  
bool is_empty(PNODE pHead)  
{  
    if(NULL==pHead->pNext)  
        return true;  
    else   
        return false;  
}  
  
int length_list(PNODE pHead)  
{  
    int len=0;  
    PNODE p=pHead->pNext;  
    while(p!=NULL)  
    {  
        p=p->pNext;  
        len++;  
    }  
    return len;  
}  
//在pHead所指向链表的第pos个节点的前面插入一个新的节点，该节点的值为val，并且pos的 值是从1开始   
bool insert_list(PNODE pHead,int val) //pos:第pos个位置，从第1个位置开始计   
{  
    PNODE p=pHead;  
      
    //不需判断链表是否为空，不需判断pos是否满足链表长度要求等。。此部分用while和if全部搞定   
    while(NULL!=p->pNext)   //不能写成<=，否则出错   
    {  
        p=p->pNext;  
    }  
  
  
    PNODE pNew=(PNODE)malloc(sizeof(NODE));  
     if(NULL==pNew)  
        {  
            cout<<"分配失败，程序终止！"<<endl;  
            exit(-1);   
        }  
          
    pNew->data =val;  
    p->pNext =pNew;  
    pNew->pNext = NULL;  
      
    return true;  
}  
  
bool delete_list(PNODE pHead,int Val)  
{  
        int i=0;  
    PNODE p=pHead->pNext; 
	PNODE pre=pHead;
	PNODE q;
      
    //不需判断链表是否为空，不需判断pos是否满足链表长度要求等。。此部分用while和if全部搞定   
	while(p != NULL)
	{
		if(p->data == Val)
		{
			q = p;
			p = p->pNext;
			pre->pNext = p;
			free(q);
		}
		else
		{
			pre = p;
			p = p->pNext;
		}
	}
	
    return true;  
}  

