/*
 *  Message Queue Manager
 *
 *
 *  COPYRIGHT (c) 1989, 1990, 1991, 1992, 1993, 1994.
 *  On-Line Applications Research Corporation (OAR).
 *  All rights assigned to U.S. Government, 1994.
 *
 *  This material may be reproduced by or for the U.S. Government pursuant
 *  to the copyright license under the clause at DFARS 252.227-7013.  This
 *  notice must appear in all copies of this file and its derivatives.
 *
 *  $Id$
 */

#include <rtems/system.h>
#include <rtems/attr.h>
#include <rtems/chain.h>
#include <rtems/config.h>
#include <rtems/isr.h>
#include <rtems/message.h>
#include <rtems/object.h>
#include <rtems/options.h>
#include <rtems/states.h>
#include <rtems/thread.h>
#include <rtems/wkspace.h>

void _Message_queue_Manager_initialization(
  unsigned32 maximum_message_queues
)
{
}

rtems_status_code rtems_message_queue_create(
  Objects_Name        name,
  unsigned32          count,
  unsigned32          max_message_size,
  rtems_attribute     attribute_set,
  Objects_Id         *id
)
{
  return( RTEMS_NOT_CONFIGURED );
}

rtems_status_code rtems_message_queue_ident(
  Objects_Name  name,
  unsigned32    node,
  Objects_Id   *id
)
{
  return( RTEMS_NOT_CONFIGURED );
}

rtems_status_code rtems_message_queue_delete(
  Objects_Id id
)
{
  return( RTEMS_NOT_CONFIGURED );
}

rtems_status_code rtems_message_queue_send(
  Objects_Id            id,
  void                 *buffer,
  unsigned32            size
)
{
  return( RTEMS_NOT_CONFIGURED );
}

rtems_status_code rtems_message_queue_urgent(
  Objects_Id            id,
  void                 *buffer,
  unsigned32            size
)
{
  return( RTEMS_NOT_CONFIGURED );
}

rtems_status_code rtems_message_queue_broadcast(
  Objects_Id            id,
  void                 *buffer,
  unsigned32            size,
  unsigned32           *count
)
{
  return( RTEMS_NOT_CONFIGURED );
}

rtems_status_code rtems_message_queue_receive(
  Objects_Id            id,
  void                 *buffer,
  unsigned32           *size_p,
  unsigned32            option_set,
  rtems_interval        timeout
)
{
  return( RTEMS_NOT_CONFIGURED );
}

rtems_status_code rtems_message_queue_flush(
  Objects_Id  id,
  unsigned32 *count
)
{
  return( RTEMS_NOT_CONFIGURED );
}

unsigned32 _Message_queue_Flush_support(
  Message_queue_Control *the_message_queue
)
{
  return 0;
}

boolean _Message_queue_Seize(
  Message_queue_Control  *the_message_queue,
  rtems_option            option_set,
  void                   *buffer,
  unsigned32             *size_p
)
{
  _Thread_Executing->Wait.return_code = RTEMS_UNSATISFIED;
  return TRUE;
}

rtems_status_code _Message_queue_Submit(
  Objects_Id                  id,
  void                       *buffer,
  unsigned32                  size,
  Message_queue_Submit_types  submit_type
)
{
  return( RTEMS_NOT_CONFIGURED );
}
