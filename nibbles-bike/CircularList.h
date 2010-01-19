#ifndef CIRCULARLIST_H
#define CIRCULARLIST_H

#define NULL 0

#include "textdisp.h"

//Basic List Node Class
template <typename T>
class CDLLNode
{
  T m_nodeData;
  CDLLNode<T>* m_prevNode;
  CDLLNode<T>* m_nextNode;
 public:
  CDLLNode(T nodeData) : 
    m_nodeData(nodeData), 
    m_prevNode(NULL), 
    m_nextNode(NULL)
      {
      }
  CDLLNode(T nodeData, CDLLNode<T>* prevNode, CDLLNode<T>* nextNode) : m_nodeData(nodeData), m_prevNode(prevNode), m_nextNode(nextNode)
    {
    }

  T GetNodeData() { return m_nodeData; }
  void SetNodeData(T nodeData) { m_nodeData = nodeData; }
  void SetNextNode(CDLLNode<T>* nextNode) { m_nextNode = nextNode; }
  void SetPrevNode(CDLLNode<T>* prevNode) { m_prevNode = prevNode; }
  CDLLNode<T>* GetPrevNode() { return m_prevNode; }
  CDLLNode<T>* GetNextNode() { return m_nextNode; }
  
};

//Circular Doubly Linked List
template <typename T>
class CDLCList
{
 protected:
  CDLLNode<T>* m_headNode;
  int m_listSize;

  void CreateInitialList(T nodeData)
  {
    m_headNode = new CDLLNode<T>(nodeData);
    m_headNode->SetPrevNode(m_headNode);
    m_headNode->SetNextNode(m_headNode);
    m_listSize = 1;
  }

 public:
  CDLCList() : m_headNode(NULL), m_listSize(0) {}
  ~CDLCList() 
    {
      
      CDLLNode<T>* temp;
      while(m_headNode != NULL && m_listSize > 0)
	{
	  temp = m_headNode;
	  m_headNode = m_headNode->GetNextNode();
	  delete temp;
	  temp = NULL;
	  --m_listSize;
	}
      
    }
  
  void InsertAfter(CDLLNode<T>* indexNode, T nodeData)
  {
    CDLLNode<T>* newNode = new CDLLNode<T>(nodeData);   
    newNode->SetPrevNode(indexNode);
    newNode->SetNextNode(indexNode->GetNextNode());
    indexNode->SetNextNode(newNode);
    newNode->GetNextNode()->SetPrevNode(newNode);
    
    ++m_listSize;
  }
  
  void InsertBefore(CDLLNode<T>* indexNode, T nodeData)
  {
    InsertAfter(indexNode->GetPrevNode(), nodeData);
  }

  void PushFront(T nodeData)
  {
    if(!m_headNode) CreateInitialList(nodeData);
    else InsertAfter(m_headNode, nodeData);
  }

  void PushBack(T nodeData)
  {
    if(!m_headNode) CreateInitialList(nodeData);
    else InsertBefore(m_headNode, nodeData);
  }

  void SetHeadNode(CDLLNode<T>* node)
  {
    m_headNode = node;
  }

  CDLLNode<T>* GetHeadNode()
    {
      return m_headNode;
    }

  int GetListSize() { return m_listSize; }

};

#endif
