/*
 * Actions.h
 *
 *  Created on: 16 apr 2019
 *      Author: jof
 */

#ifndef ACTIONS_H_
#define ACTIONS_H_
#include <stdint.h>

class Segment;
class SegmentCompound;

/**
 * @brief: Base class for all actions
 */
class ActionBase {
protected:
  bool m_singleShot;
  unsigned long m_time;
  Segment *m_segment;
  SegmentCompound *m_segmentCompound;

public:
  explicit ActionBase(Segment *segment);
  explicit ActionBase(SegmentCompound *compound);
  virtual ~ActionBase();

  bool isRunning() const;
  virtual bool isFinished() const = 0;

  virtual void loop() = 0; // subclass must implement
};

#endif /* ACTIONS_H_ */
