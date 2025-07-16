#include <TestUtils.h>
#include <fstream>
#include <gtest/gtest.h>
#include <schema/attributes/Partition.h>

static std::ifstream inFile256{test_utils::JSON_Test_Schemas.at("256").c_str()};
static inline const auto schema256JSON = nlohmann::json::parse(inFile256);

// Schema functionality

TEST(Partition, ValidatesFieldLengthSumAgainstLengthKey)
{
  const auto _json = nlohmann::json::parse(R"(
    {
    "magic":{
        "offset": 0,
        "length": 16,
        "fields":[
            {"name": "header1","length": 8, "endian":"big"},
            {"name": "header2","length": 8, "endian":"big"},
            {"name": "header3","length": 8, "endian":"big"}
        ]
    }
    }
    )");
  ASSERT_ANY_THROW(Partition::fromJSON(_json));
}

TEST(Partition, JSONFileConstruction)
{
  const auto hdr = Partition::fromJSON(
      schema256JSON.front()); // Header

  ASSERT_EQ(hdr.start_offset(), 0);

  auto payload = Partition::fromJSON(
      schema256JSON.back()); // payload
  ASSERT_EQ(payload.start_offset(), 16);
}

TEST(Partition, PrettyPrintable)
{
  auto part = Partition::fromJSON(
      schema256JSON.front()); // Header
  std::cout << part << std::endl;
}