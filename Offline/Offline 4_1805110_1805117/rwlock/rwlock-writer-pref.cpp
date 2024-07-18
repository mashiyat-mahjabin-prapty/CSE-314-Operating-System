#include "rwlock.h"

void InitalizeReadWriteLock(struct read_write_lock * rw)
{
  //	Write the code for initializing your read-write lock.
  rw->readers = 0;
  rw->writers = 0;
  rw->waiting_reader = 0;
  rw->waiting_writer = 0;
  pthread_cond_init(&rw->writelock, NULL);
  pthread_cond_init(&rw->readlock, NULL);
  pthread_mutex_init(&rw->lock, NULL);
}

void ReaderLock(struct read_write_lock * rw)
{
  //	Write the code for aquiring read-write lock by the reader.
  pthread_mutex_lock(&rw->lock);

  if(rw->writers == 1 || rw->waiting_writer > 0)
  {
    rw->waiting_reader++;

    pthread_cond_wait(&rw->readlock, &rw->lock);

    rw->waiting_reader--;
  }

  rw->readers++;
  pthread_mutex_unlock(&rw->lock);
  pthread_cond_broadcast(&rw->readlock); // ami jehetu lock paisi tai amar bondhu bandhob shobaike deke niye ashbo
}

void ReaderUnlock(struct read_write_lock * rw)
{
  //	Write the code for releasing read-write lock by the reader.
  pthread_mutex_lock(&rw->lock);
 
  rw->readers--;
  if (rw->readers == 0)
    pthread_cond_signal(&rw->writelock);

  pthread_mutex_unlock(&rw->lock);
}

void WriterLock(struct read_write_lock * rw)
{
  //	Write the code for aquiring read-write lock by the writer.
  pthread_mutex_lock(&rw->lock);

  rw->waiting_writer++;

  while(rw->writers || rw->readers)
  {
    pthread_cond_wait(&rw->writelock, &rw->lock);
  }

  rw->waiting_writer--;
  rw->writers = 1;

  pthread_mutex_unlock(&rw->lock);
}

void WriterUnlock(struct read_write_lock * rw)
{
  //	Write the code for releasing read-write lock by the writer.
  pthread_mutex_lock(&rw->lock);
  rw->writers = 0;
 
  if (rw->waiting_writer > 0)
    pthread_cond_signal(&rw->writelock);
  else
    pthread_cond_signal(&rw->readlock);
  pthread_mutex_unlock(&rw->lock);
}
