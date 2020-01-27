/*******************************************************************************
 *
 *
 * Purpose: The purpose of this benchmark is to see what the output write
 *          performance of the Luna Log system is compared to using syslog()
 *          directly.
 *
 * Notes:   The Luna log system temporarily cache messages while performing the
 *          writting of log messages in a diffirent thread. Thus there are
 *          really two tests to perform
 *
 ******************************************************************************/
#include "../Include/Luna/Common/Debug/Log.hpp"
#include <benchmark/benchmark.h>
#include <syslog.h>
#include <unistd.h>

#define LUNA_BM_DELAY_AFTER_BM 5

/******************************************************************************/
void gLunaBM_Log(benchmark::State &state)
{
  /* The first time the log function is called the log instance is created. Need
   * to make sure that it's not optimised out since this allocated the
   * memory. */
  benchmark::DoNotOptimize(Luna::Log::ptr());
  benchmark::ClobberMemory();

  int i = 0;
  for(auto _ : state)
  {
    /* Write the log entries. */
    LUNA_LOG_NOTICE("Benchmark String: " << i++)
  }
  state.PauseTiming();

  /* Give some time for syslog to finish writting the current batch of data
   * before starting the next benchmark, else we are likely still measuring the
   * results from this benchmark in the next one.*/
  sleep(LUNA_BM_DELAY_AFTER_BM);

  state.ResumeTiming();
}

/******************************************************************************/
void gLunaBM_Syslog(benchmark::State &state)
{
  /* Syslog is allready configtured inside of Luna::Log. Only need to start
   * writting. */
  int i = 0;
  for(auto _ : state)
  {
    /* Write the log entries. */
    syslog(LOG_NOTICE, "%s%d", "Benchmark String: ", i++);
  }

  state.PauseTiming();
  /* Give some time for syslog to finish writting the current batch of data
   * before starting the next benchmark, else we are likely still measuring the
   * results from this benchmark in the next one.*/
  sleep(LUNA_BM_DELAY_AFTER_BM);
  state.ResumeTiming();
}

BENCHMARK(gLunaBM_Log)->Iterations(1);
BENCHMARK(gLunaBM_Syslog)->Iterations(1);

BENCHMARK(gLunaBM_Log)->Iterations(10);
BENCHMARK(gLunaBM_Syslog)->Iterations(10);

BENCHMARK(gLunaBM_Log)->Iterations(100);
BENCHMARK(gLunaBM_Syslog)->Iterations(100);

BENCHMARK(gLunaBM_Log)->Iterations(1000);
BENCHMARK(gLunaBM_Syslog)->Iterations(1000);

BENCHMARK(gLunaBM_Log)->Iterations(10000);
BENCHMARK(gLunaBM_Syslog)->Iterations(10000);

BENCHMARK_MAIN();
