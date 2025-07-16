#include <gtest/gtest.h>

#include <schema/Schema.h>
#include <schema/Mapper.h>
#include <schema/results/Results.h>

#include <TestUtils.h>

static std::ifstream inFile256{test_utils::JSON_Test_Schemas.at("256").c_str()};
static inline auto schema256JSON = nlohmann::json::parse(inFile256);
static test_utils::GenerateBinaryData<256> generateMock256Data()
{
  auto data = test_utils::GenerateBinaryData<256>();
  auto dataWriter = test_utils::WriteStrategy<256>(
      [](const auto &i, uint8_t &c)
      {
        c = i % 255;
      });

  data += dataWriter;
  return data;
}
TEST(SchemaMapper, ValidatesInputDataLength)
{

  const auto schema = Schema::fromJSON(schema256JSON);
  auto data = generateMock256Data();

  ResultsBuilder builder;
  for (const auto &part : schema)
    builder.add(part, data);

  std::cout << builder.results() << std::endl;
}