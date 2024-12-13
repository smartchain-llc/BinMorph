#include <gtest/gtest.h>
#include <TestUtils.h>

#include <schema/Parser.h>
#include <schema/SchemaResults.h>

using namespace bm;
using namespace nlohmann;




TEST(BitSignificance, MSBandLSB){
    ///MSB: 0XFF ; LSB: 0X00
    uint64_t input = 0xFFAAFFBB00;
    /**
    Stored in-memory byte positions:     0  1  2  3  4
                                      0x00 BB FF AA FF
                                      Stored as little endian
     */
    uint64_t* inptr = &input;
}
struct Output{
    Output(const SchemaDataMapper& sm, std::filesystem::path out){
        std::ofstream of { out };
        for(const auto& j : sm.results())
            of << j;
        of.flush();
    }
};
TEST(BinSchemaResults256, MapsBinAgainstSchema){
    auto data = test_utils::GenerateBinaryData<256>();
    auto DataWriter = test_utils::WriteStrategy<256>([](const auto& i, uint8_t& c){
        if(i < 16)
            c = (i % 3) ? i : 0x01;
        if(i < (256-16) && i > 16){
            if(i % 4)
                c = 0x04;
            if(i % 6)
                c = 0x69;
            if(i % 9)
                c = 0xFF;
        }
    });
    data += DataWriter;
    test_utils::CreateAndWrite(std::filesystem::path(TEST_DATA_PATH) / "256.bin", data, 256);

    Schema schema;
    Parser::ParseTo(schema, test_utils::JSONFrom("256"));
    SchemaDataMapper mapper {schema, data};
    
    std::ofstream outFile { std::filesystem::path(TEST_DATA_PATH) / "256_results.json", std::ios::trunc};
    for(const auto& j : mapper.results())
        outFile << j;
    outFile.flush();
    ASSERT_TRUE(data != nullptr);
}

// TEST(BinaryDataFromSchema, CanReadBinaryDataAccordingToSchema)
// {
//     Schema schema;
//     Parser::ParseTo(schema, ValidJSON);

//     // Stored Data: FF 69 FF 69... from little endian value: 0x69 FF 69 FF...
//     auto data = BinaryData();
//     BinData<32> binData;

//     for(const auto& part : schema){
//         // Write to stored w/ Big endian: `FF 69 FF 69...`
//         //                   Address N+i:  0  1  2  3
//         //                   Address N+i:  0  1  2  3
         
//         Reader::read(part, data, binData.data);
//     }
    
//     uint64_t* head = (uint64_t*)(binData.data);
//     /// In memory as: `0xFF 69 FF 69 FF 69 FF 69`
//     // uint64_t expected = 0x69FF69FF69FF69FF;
//     BinData<8> expected {{0x69, 0xFF, 0x69, 0xFF, 0x69, 0xFF, 0x69, 0xFF}}; // Expected needs to be same as raw input, not in-mem storage
    
//     // ASSERT_EQ(*head, 0x69FF69FF69FF69FF);
//     ASSERT_TRUE(BinaryCompare(binData.parition<8>(1),expected));
// }
