#include <stdio.h>

#include "mongoc-array-private.h"
#include "mongoc-thread-private.h"
#include "future.h"

/**************************************************
 *
 * Generated by build/generate-future-functions.py.
 *
 * DO NOT EDIT THIS FILE.
 *
 *************************************************/

#define FUTURE_TIMEOUT_MS 10 * 1000

void
future_get_void (future_t *future)
{
   if (!future_wait (future)) {
      fprintf (stderr, "%s timed out\n", __FUNCTION__);
      abort ();
   }
}


bool
future_get_bool (future_t *future)
{
   if (future_wait (future)) {
      return future_value_get_bool (&future->return_value);
   }

   fprintf (stderr, "%s timed out\n", __FUNCTION__);
   abort ();
}

char_ptr
future_get_char_ptr (future_t *future)
{
   if (future_wait (future)) {
      return future_value_get_char_ptr (&future->return_value);
   }

   fprintf (stderr, "%s timed out\n", __FUNCTION__);
   abort ();
}

char_ptr_ptr
future_get_char_ptr_ptr (future_t *future)
{
   if (future_wait (future)) {
      return future_value_get_char_ptr_ptr (&future->return_value);
   }

   fprintf (stderr, "%s timed out\n", __FUNCTION__);
   abort ();
}

int64_t
future_get_int64_t (future_t *future)
{
   if (future_wait (future)) {
      return future_value_get_int64_t (&future->return_value);
   }

   fprintf (stderr, "%s timed out\n", __FUNCTION__);
   abort ();
}

uint32_t
future_get_uint32_t (future_t *future)
{
   if (future_wait (future)) {
      return future_value_get_uint32_t (&future->return_value);
   }

   fprintf (stderr, "%s timed out\n", __FUNCTION__);
   abort ();
}

const_char_ptr
future_get_const_char_ptr (future_t *future)
{
   if (future_wait (future)) {
      return future_value_get_const_char_ptr (&future->return_value);
   }

   fprintf (stderr, "%s timed out\n", __FUNCTION__);
   abort ();
}

bson_error_ptr
future_get_bson_error_ptr (future_t *future)
{
   if (future_wait (future)) {
      return future_value_get_bson_error_ptr (&future->return_value);
   }

   fprintf (stderr, "%s timed out\n", __FUNCTION__);
   abort ();
}

bson_ptr
future_get_bson_ptr (future_t *future)
{
   if (future_wait (future)) {
      return future_value_get_bson_ptr (&future->return_value);
   }

   fprintf (stderr, "%s timed out\n", __FUNCTION__);
   abort ();
}

const_bson_ptr
future_get_const_bson_ptr (future_t *future)
{
   if (future_wait (future)) {
      return future_value_get_const_bson_ptr (&future->return_value);
   }

   fprintf (stderr, "%s timed out\n", __FUNCTION__);
   abort ();
}

const_bson_ptr_ptr
future_get_const_bson_ptr_ptr (future_t *future)
{
   if (future_wait (future)) {
      return future_value_get_const_bson_ptr_ptr (&future->return_value);
   }

   fprintf (stderr, "%s timed out\n", __FUNCTION__);
   abort ();
}

mongoc_bulk_operation_ptr
future_get_mongoc_bulk_operation_ptr (future_t *future)
{
   if (future_wait (future)) {
      return future_value_get_mongoc_bulk_operation_ptr (&future->return_value);
   }

   fprintf (stderr, "%s timed out\n", __FUNCTION__);
   abort ();
}

mongoc_client_ptr
future_get_mongoc_client_ptr (future_t *future)
{
   if (future_wait (future)) {
      return future_value_get_mongoc_client_ptr (&future->return_value);
   }

   fprintf (stderr, "%s timed out\n", __FUNCTION__);
   abort ();
}

mongoc_cursor_ptr
future_get_mongoc_cursor_ptr (future_t *future)
{
   if (future_wait (future)) {
      return future_value_get_mongoc_cursor_ptr (&future->return_value);
   }

   fprintf (stderr, "%s timed out\n", __FUNCTION__);
   abort ();
}

mongoc_database_ptr
future_get_mongoc_database_ptr (future_t *future)
{
   if (future_wait (future)) {
      return future_value_get_mongoc_database_ptr (&future->return_value);
   }

   fprintf (stderr, "%s timed out\n", __FUNCTION__);
   abort ();
}

mongoc_query_flags_t
future_get_mongoc_query_flags_t (future_t *future)
{
   if (future_wait (future)) {
      return future_value_get_mongoc_query_flags_t (&future->return_value);
   }

   fprintf (stderr, "%s timed out\n", __FUNCTION__);
   abort ();
}

const_mongoc_read_prefs_ptr
future_get_const_mongoc_read_prefs_ptr (future_t *future)
{
   if (future_wait (future)) {
      return future_value_get_const_mongoc_read_prefs_ptr (&future->return_value);
   }

   fprintf (stderr, "%s timed out\n", __FUNCTION__);
   abort ();
}


future_t *
future_new (future_value_type_t return_type, int argc)
{
   future_t *future;

   future = bson_malloc0 (sizeof *future);
   future->return_value.type = return_type;
   future->argc = argc;
   future->argv = bson_malloc0 ((size_t) argc * sizeof(future_value_t));
   mongoc_cond_init (&future->cond);
   mongoc_mutex_init (&future->mutex);

   return future;
}

future_value_t *
future_get_param (future_t *future, int i)
{
   return &future->argv[i];
}

future_t *
future_new_copy (future_t *future)
{
   future_t *copy;

   mongoc_mutex_lock (&future->mutex);
   copy = future_new (future->return_value.type, future->argc);
   copy->return_value = future->return_value;
   memcpy (copy->argv, future->argv, future->argc * sizeof(future_value_t));
   mongoc_mutex_unlock (&future->mutex);

   return copy;
}


void
future_start (future_t *future,
              void *(*start_routine)(void *))
{
   int r = mongoc_thread_create (&future->thread,
                                 start_routine,
                                 (void *) future);

   assert (!r);
}


void
future_resolve (future_t *future, future_value_t return_value)
{
   mongoc_mutex_lock (&future->mutex);
   assert (!future->resolved);
   assert (future->return_value.type == return_value.type);
   future->return_value = return_value;
   future->resolved = true;
   mongoc_cond_signal (&future->cond);
   mongoc_mutex_unlock (&future->mutex);
}


bool
future_wait (future_t *future)
{
   /* TODO: configurable timeout */
   int64_t deadline = bson_get_monotonic_time () + FUTURE_TIMEOUT_MS * 1000;
   bool resolved;

   mongoc_mutex_lock (&future->mutex);
   while (!future->resolved && bson_get_monotonic_time () <= deadline) {
      mongoc_cond_timedwait (&future->cond, &future->mutex, FUTURE_TIMEOUT_MS);
   }
   resolved = future->resolved;
   mongoc_mutex_unlock (&future->mutex);

   return resolved;
}


void
future_destroy (future_t *future)
{
   bson_free (future->argv);
   mongoc_cond_destroy (&future->cond);
   mongoc_mutex_destroy (&future->mutex);
   bson_free (future);
}
