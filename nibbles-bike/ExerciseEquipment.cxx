#include "ExerciseEquipment.h"
#include "textdisp.h"

CExerciseEquipment::CExerciseEquipment(int historyLength) : m_historyLength(historyLength), m_totalTickCount(0)
{
  //Preallocate for historyLength
  while(m_tickHistoryList.GetListSize() < m_historyLength)
    {
      m_tickHistoryList.PushBack(0);
    }
  m_listCursor = m_tickHistoryList.GetHeadNode();
}

CExerciseEquipment::~CExerciseEquipment()
{
}

void CExerciseEquipment::AddTickPeriod(int tickCount)
{
  m_totalTickCount += tickCount;
  m_listCursor->SetNodeData(tickCount);
  m_listCursor = m_listCursor->GetNextNode();  
}

float CExerciseEquipment::GetVelocity(int tickPeriod)
{
  int listSize = m_tickHistoryList.GetListSize();
  if(!listSize || !tickPeriod) return 0;
  if(tickPeriod > listSize) return ((float)GetTickCount(tickPeriod))/(float)listSize;
  return ((float)GetTickCount(tickPeriod))/(float)tickPeriod;
}

int CExerciseEquipment::GetTickCount(int tickPeriod)
{
  int tickCount = 0;
  int i = 0;
  int listSize = m_tickHistoryList.GetListSize();
  CDLLNode<int>* reverseItr = m_listCursor->GetPrevNode();
  for(i=0; i<tickPeriod; ++i)
    {
      if(i > listSize) break;
      tickCount += reverseItr->GetNodeData();
      reverseItr = reverseItr->GetPrevNode();
    }
  return tickCount;
}
