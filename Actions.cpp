/*
 * Actions.cpp
 *
 *  Created on: 16 apr 2019
 *      Author: jof
 */


#include "Actions.h"
#include "Leds.h"

ActionsContainer::ActionsContainer() :
  m_currentIdx(0)
{
}

ActionsContainer::~ActionsContainer()
{
}

size_t ActionsContainer::actionsSize() const
{
  return m_actions.length();
}

void ActionsContainer::actionsPush(ActionBase *action)
{
  m_actions.push(action);
}

void ActionsContainer::actionsRemove(ActionBase *action)
{
  for(uint16_t i = 0; i < m_actions.length(); ++i) {
    if (m_actions[i] == action) {
      m_actions.remove(i);
      if (m_currentIdx == i) {
        if (m_actions.length() -1 <= i)
          actionsRestart();
      } else if (m_currentIdx > i)
        --m_currentIdx;
    }
  }
}

void ActionsContainer::actionsRestart()
{
  m_currentIdx = 0;
}

void ActionsContainer::actionsNext()
{
  ++m_currentIdx;
}

ActionBase* ActionsContainer::actionsCurrent()
{
  if (m_actions.length())
    return m_actions[m_currentIdx];
  return nullptr;
}

void ActionsContainer::nextAction()
{
  ++m_currentIdx;
  if (m_actions.length() == m_currentIdx)
    m_currentIdx = 0;
}

// -------------------------------------------------------

ActionBase::ActionBase(SegmentCommon *owner) :
  m_singleShot(false), m_startTime(0),
  m_duration(0), m_owner(owner)
{
}

ActionBase::~ActionBase()
{
}

bool ActionBase::isRunning() const
{
  return m_startTime > 0 && !isFinished();
}

bool ActionBase::isFinished() const
{
  // duration of 0 is forever
  return m_duration > 0 && m_startTime + m_duration >= millis();
}

void ActionBase::loop()
{
  if (m_startTime == 0)
    m_startTime = millis();

  if (isFinished()) {
    if (m_owner)
      m_owner->nextAction();
  }
}

// -----------------------------------------------

ActionDark::ActionDark(SegmentCommon *owner) :
    ActionBase(owner)
{
}

ActionDark::~ActionDark()
{
}

void ActionDark::loop()
{
  if (m_startTime == 0) {
    for(uint16_t i = 0; i < m_owner->size(); ++i) {
      CRGB *rgb = (*m_owner)[i];
      *rgb = CRGB::Black;
    }
  }

  ActionBase::loop();
}


