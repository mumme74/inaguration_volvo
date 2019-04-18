/*
 * Actions.h
 *
 *  Created on: 16 apr 2019
 *      Author: jof
 */

#ifndef ACTIONS_H_
#define ACTIONS_H_
#include <DList.h>
#include <stdint.h>
#include <FastLED.h>

class SegmentCommon;
class ActionBase;


/**
 * @brief: base class for Segment and SegmentCompound
 *         implements actions logic
 */
class ActionsContainer {
protected:
  DListDynamic<ActionBase*> m_actions;
  uint16_t m_currentIdx;
public:
  explicit ActionsContainer();
  virtual ~ActionsContainer();

  // Actions
  size_t actionsSize() const;
  void actionsPush(ActionBase *action);
  void actionsRemove(ActionBase *action);
  void actionsRestart();
  void actionsNext();
  ActionBase* actionsCurrent();
  void nextAction();
};

// ----------------------------------------------------------

/**
 * @brief: Base class for all actions
 */
class ActionBase {
protected:
  bool m_singleShot;
  unsigned long m_startTime;
  uint32_t m_duration;
  SegmentCommon *m_owner;

public:
  explicit ActionBase(SegmentCommon *owner);
  virtual ~ActionBase();

  /// action takes this long in milliseconds
  /// duration of 0 is a forever action
  uint32_t duration() const { return m_duration; }
  void setDuration(uint32_t duration) { m_duration = duration; }

  virtual bool isRunning() const;
  virtual bool isFinished() const;

  virtual void loop(); // subclass must implement functionality
};

// ----------------------------------------------------

class ActionColor : public ActionBase {
  CRGB m_color;
public:
  explicit ActionColor(SegmentCommon *owner, CRGB color);
  virtual ~ActionColor();

  void loop();
};

// ----------------------------------------------------

class ActionDark : public ActionColor {
public:
  explicit ActionDark(SegmentCommon *owner);
};


#endif /* ACTIONS_H_ */
