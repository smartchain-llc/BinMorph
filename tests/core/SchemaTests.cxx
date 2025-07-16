#include <TestUtils.h>
#include <fstream>
#include <gtest/gtest.h>

#include <schema/Schema.h>

static std::ifstream inFile256{test_utils::JSON_Test_Schemas.at("256").c_str()};
static inline auto schema256JSON = nlohmann::json::parse(inFile256);
// static std::ifstream inFileOverlap{test_utils::JSON_Test_Schemas.at("overlap").c_str()};
// static inline auto schemaOverlapJSON = nlohmann::json::parse(inFileOverlap);

TEST(Schema, CreatedFromListOfPartitions)
{
  const auto schema = Schema::fromJSON(schema256JSON);
  ASSERT_EQ(schema.getPartition(0).start_offset(), 0);
  ASSERT_EQ(schema.getPartition(0).end_offset(), 15);
  ASSERT_EQ(schema.getPartition(1).start_offset(), 16);
  ASSERT_EQ(schema.getPartition(1).end_offset(), (16 + 240 - 1));
}

TEST(Schema, CanDetectPartitionOverlap)
{
  const auto overlappingJSON = nlohmann::json::parse(R"({
  "part1": {
    "offset": 0,
    "fields": [
      { "name": "na", "length": 8, "endian": "little" },
      { "name": "na", "length": 8, "endian": "little" }
    ]
  },
  "overlap": {
    "offset": 8,
    "fields": [
      { "name": "na", "length": 8, "endian": "little" },
      { "name": "na", "length": 8, "endian": "little" }
    ]
  }
})");

  ASSERT_ANY_THROW(
      Schema::fromJSON(overlappingJSON););
}

TEST(Schema, CanProcedurallyCalculatesLength)
{
  const auto schema = Schema::fromJSON(schema256JSON);
  ASSERT_EQ(schema.getPartition(0).length(), 16);
}

// TEST(Schema, CanMapBinaryDataAgainstSchema)
// {
//   const auto schema = Schema::fromJSON(schema256JSON);

//   auto data = test_utils::GenerateBinaryData<256>();
//   auto dataWriter = test_utils::WriteStrategy<256>(
//       [](const auto &i, uint8_t &c)
//       {
//         c = i % 255;
//       });

//   data += dataWriter;

//   struct Header
//   {
//     uint8_t data[16] = {0};
//   };

//   auto hData = std::bit_cast<Header *>((uint8_t *)data);
//   for (auto i = 0; i < 16; i++)
//   {
//     uint8_t cmp = (*(uint8_t *)data) + i;
//     ASSERT_EQ(hData->data[i], cmp);
//   }
// }

// TEST(Schema, CanMapDataToSchemaByteType)
// {
//   const auto schema = Schema::fromJSON(schema256JSON);

//   // Schema Impl
//   for (const auto &part : schema)
//   {
//   }
// }