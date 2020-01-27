#include <benchmark/benchmark.h>
#include "../Include/Luna/Common/ReusableID.hpp"

#define LUNA_BM_NUM_ITERATIONS    1000000

static Luna::ReusableID<uint32_t> gLunaBM_RID(LUNA_BM_NUM_ITERATIONS);
static std::vector<uint32_t> gLunaBM_RIDResults;

static void gLunaBM_ReusableID_Next(benchmark::State & state)
{
  gLunaBM_RIDResults.resize(LUNA_BM_NUM_ITERATIONS);
  uint32_t curIndex = 0;

  for(auto _ : state)
  {
      gLunaBM_RIDResults.data()[curIndex++] = gLunaBM_RID.next().value();
  }
}

BENCHMARK(gLunaBM_ReusableID_Next)->Iterations(LUNA_BM_NUM_ITERATIONS);

static void gLunaBM_ReusableID_Release(benchmark::State & state)
{
  uint32_t curIndex = 0;
  for(auto _ : state)
  {
      gLunaBM_RID.release(curIndex++);
  }
}

BENCHMARK(gLunaBM_ReusableID_Release)->Iterations(LUNA_BM_NUM_ITERATIONS);

BENCHMARK_MAIN();
