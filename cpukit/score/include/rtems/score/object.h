/*  object.h
 *
 *  This include file contains all the constants and structures associated
 *  with the RTEMS Object Handler.  This Handler provides mechanisms which
 *  can be used to initialize and manipulate all RTEMS objects.
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

#ifndef __RTEMS_OBJECTS_h
#define __RTEMS_OBJECTS_h

#ifdef __cplusplus
extern "C" {
#endif

#include <rtems/chain.h>

/*
 *  The following type defines the control block used to manage
 *  object names.
 */

typedef unsigned32 Objects_Name;

/*
 *  The following type defines the control block used to manage
 *  object IDs.
 */

typedef unsigned32 Objects_Id;

/*
 *  This enumerated type lists the locations which may be returned
 *  by _Objects_Get.  These codes indicate the success of locating
 *  an object with the specified ID.
 */

typedef enum {
  OBJECTS_LOCAL  = 0,         /* object is local */
  OBJECTS_REMOTE = 1,         /* object is remote */
  OBJECTS_ERROR  = 2          /* id was invalid */
}  Objects_Locations;

/*
 *  The following defines the Object Control Block used to manage
 *  each object local to this node.
 */

typedef struct {
  Chain_Node Node;
  Objects_Id id;
}   Objects_Control;

/*
 *  The following defines the structure for the information used to
 *  manage each class of objects.
 */

typedef struct {
  Objects_Id        minimum_id;      /* minimum valid id of this type */
  Objects_Id        maximum_id;      /* maximum valid id of this type */
  unsigned32        maximum;         /* maximum number of objects */
  Objects_Control **local_table;     /* table of local object pointers */
  Objects_Name     *name_table;      /* table of local object names */
  Chain_Control    *global_table;    /* pointer to global table */
  Chain_Control     Inactive;        /* chain of inactive ctl blocks */
}   Objects_Information;

/*
 *  The following defines the data storage which contains the
 *  node number of the local node.
 */

EXTERN unsigned32  _Objects_Local_node;

/*
 *  The following defines the constant which may be used
 *  with _Objects_Get to manipulate the calling task.
 *
 */

#define OBJECTS_ID_OF_SELF 0

/*
 *  The following define the constants which may be used in name searches.
 */

#define RTEMS_SEARCH_ALL_NODES   0
#define RTEMS_SEARCH_OTHER_NODES 0x7FFFFFFE
#define RTEMS_SEARCH_LOCAL_NODE  0x7FFFFFFF
#define RTEMS_WHO_AM_I           0

/*
 * Parameters and return id's for _Objects_Get_next
 */

#define RTEMS_OBJECT_ID_INITIAL_INDEX   (0)
#define RTEMS_OBJECT_ID_FINAL_INDEX     (0xffff)

#define RTEMS_OBJECT_ID_INITIAL(node)   (_Objects_Build_id(      \
                                            node, \
                                            RTEMS_OBJECT_ID_INITIAL_INDEX))
#define RTEMS_OBJECT_ID_FINAL           ((Objects_Id) ~0)

/*
 *  _Objects_Handler_initialization
 *
 *  DESCRIPTION:
 *
 *  This function performs the initialization necessary for this handler.
 *
 */

void _Objects_Handler_initialization(
  unsigned32 node,
  unsigned32 maximum_global_objects
);

/*
 *  _Objects_Initialize_information
 *
 *  DESCRIPTION:
 *
 *  This function initializes an object class information record.
 *  SUPPORTS_GLOBAL is TRUE if the object class supports global
 *  objects, and FALSE otherwise.  Maximum indicates the number
 *  of objects required in this class and size indicates the size
 *  in bytes of each control block for this object class.
 *
 */

void _Objects_Initialize_information (
  Objects_Information *information,
  boolean              supports_global,
  unsigned32           maximum,
  unsigned32           size
);

/*
 *  _Objects_Name_to_id
 *
 *  DESCRIPTION:
 *
 *  This function implements the common portion of the object
 *  identification directives.  This directive returns the object
 *  id associated with name.  If more than one object of this class
 *  is named name, then the object to which the id belongs is
 *  arbitrary.  Node indicates the extent of the search for the
 *  id of the object named name.  If the object class supports global
 *  objects, then the search can be limited to a particular node
 *  or allowed to encompass all nodes.
 *
 */

rtems_status_code _Objects_Name_to_id(
  Objects_Information *information,
  Objects_Name         name,
  unsigned32           node,
  Objects_Id          *id
);

/*
 *  _Objects_Get
 *
 *  DESCRIPTION:
 *
 *  This function maps object ids to object control blocks.
 *  If id corresponds to a local object, then it returns
 *  the_object control pointer which maps to id and location
 *  is set to OBJECTS_LOCAL.  If the object class supports global
 *  objects and the object id is global and resides on a remote
 *  node, then location is set to OBJECTS_REMOTE, and the_object
 *  is undefined.  Otherwise, location is set to OBJECTS_ERROR
 *  and the_object is undefined.
 *
 */

Objects_Control *_Objects_Get (
  Objects_Information *information,
  Objects_Id           id,
  Objects_Locations   *location
);

/*
 *  _Objects_Get_next
 *
 *  DESCRIPTION:
 *
 *  Like _Objects_Get, but is used to find "next" open object.
 *
 */

Objects_Control *_Objects_Get_next(
    Objects_Information *information,
    Objects_Id           id,
    unsigned32          *location_p,
    Objects_Id          *next_id_p
);

/*
 *  _Objects_Is_name_valid
 *
 *  DESCRIPTION:
 *
 *  This function returns TRUE if the name is valid, and FALSE otherwise.
 */

STATIC INLINE boolean _Objects_Is_name_valid (
  Objects_Name name
);

/*
 *  rtems_build_name
 *
 *  DESCRIPTION:
 *
 *  This function returns an object name composed of the four characters
 *  C1, C2, C3, and C4.
 *
 *  NOTE:
 *
 *  This must be implemented as a macro for use in Configuration Tables.
 *
 */

#define rtems_build_name( _C1, _C2, _C3, _C4 ) \
  ( (_C1) << 24 | (_C2) << 16 | (_C3) << 8 | (_C4) )

/*
 *  rtems_name_to_characters
 *
 *  DESCRIPTION:
 *
 *  This function breaks the object name into the four component
 *  characters C1, C2, C3, and C4.
 *
 */

STATIC INLINE void rtems_name_to_characters(
  Objects_Name  name,
  char         *c1,
  char         *c2,
  char         *c3,
  char         *c4
);

/*
 *  _Objects_Build_id
 *
 *  DESCRIPTION:
 *
 *  This function builds an object's id from the processor node and index
 *  values specified.
 *
 */

STATIC INLINE Objects_Id _Objects_Build_id(
  unsigned32 node,
  unsigned32 index
);

/*
 *  rtems_get_node
 *
 *  DESCRIPTION:
 *
 *  This function returns the node portion of the ID.
 *
 */

STATIC INLINE unsigned32 rtems_get_node(
  Objects_Id id
);

/*
 *  rtems_get_index
 *
 *  DESCRIPTION:
 *
 *  This function returns the index portion of the ID.
 *
 */

STATIC INLINE unsigned32 rtems_get_index(
  Objects_Id id
);

/*
 *  _Objects_Is_local_node
 *
 *  DESCRIPTION:
 *
 *  This function returns TRUE if the node is of the local object, and
 *  FALSE otherwise.
 *
 */

STATIC INLINE boolean _Objects_Is_local_node(
  unsigned32 node
);

/*
 *  _Objects_Is_local_id
 *
 *  DESCRIPTION:
 *
 *  This function returns TRUE if the id is of a local object, and
 *  FALSE otherwise.
 *
 */

STATIC INLINE boolean _Objects_Is_local_id(
  Objects_Id id
);

/*
 *  _Objects_Are_ids_equal
 *
 *  DESCRIPTION:
 *
 *  This function returns TRUE if left and right are equal,
 *  and FALSE otherwise.
 *
 */

STATIC INLINE boolean _Objects_Are_ids_equal(
  Objects_Id left,
  Objects_Id right
);

/*
 *  _Objects_Allocate
 *
 *  DESCRIPTION:
 *
 *  This function allocates a object control block from
 *  the inactive chain of free object control blocks.
 *
 */

STATIC INLINE Objects_Control *_Objects_Allocate(
  Objects_Information *information
);

/*
 *  _Objects_Free
 *
 *  DESCRIPTION:
 *
 *  This function frees a object control block to the
 *  inactive chain of free object control blocks.
 *
 */

STATIC INLINE void _Objects_Free(
  Objects_Information *information,
  Objects_Control     *the_object
);

/*
 *  _Objects_Open
 *
 *  DESCRIPTION:
 *
 *  This function places the_object control pointer and object name
 *  in the Local Pointer and Local Name Tables, respectively.
 *
 */

STATIC INLINE void _Objects_Open(
  Objects_Information *information,
  Objects_Control     *the_object,
  Objects_Name         name
);

/*
 *  _Objects_Close
 *
 *  DESCRIPTION:
 *
 *  This function removes the_object control pointer and object name
 *  in the Local Pointer and Local Name Tables.
 *
 */

STATIC INLINE void _Objects_Close(
  Objects_Information *information,
  Objects_Control     *the_object
);

#include <rtems/object.inl>
#include <rtems/objectmp.h>

#ifdef __cplusplus
}
#endif

#endif
/* end of include file */
