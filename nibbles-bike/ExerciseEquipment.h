#ifndef EXERCISEEQUIPMENT_H
#define EXERCISEEQUIPMENT_H

#include "CircularList.h"

class CExerciseEquipment
{
 protected:
  CDLCList<int> m_tickHistoryList;
  CDLLNode<int>* m_listCursor;
  const int m_historyLength;
  int m_totalTickCount;
  float m_ticksPerFoot;

 public:
  CExerciseEquipment(int historyLength);
  ~CExerciseEquipment();
  float GetVelocity(int tickPeriod);
  int GetTickCount(int tickPeriod);
  void AddTickPeriod(int tickAmount);

  int GetTotalTickCount() { return m_totalTickCount; }
  //  void SetHistoryLength(int historyLength) { m_historyLength = historyLength; }
  int GetHistoryLength() { return m_historyLength; }
  
  void SetTicksPerFoot(float ticksPerFoot) { m_ticksPerFoot = ticksPerFoot; }

};

#endif
