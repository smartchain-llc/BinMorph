#include <gtest/gtest.h>
#include <TestUtils.h>
#include <schema/Fields.h>

TEST(TestUtils_GenerateData, GeneratesDataAccordingToWriteStrategy){
    auto data = test_utils::GenerateBinaryData<256>();
    auto WriteIndexValue = test_utils::WriteStrategy<16>{[](const std::size_t& i, uint8_t& c){ 
        c = i; 
    }};
    auto WriteIfMod_5 = test_utils::WriteStrategy<257>{[](const std::size_t& i, uint8_t& c){ 
        if(i % 5 == 0) c = i;
        else c = 0xFF; 
    }};

    data += WriteIndexValue;
    data += WriteIfMod_5;
    auto readme = data();
    // data += [](uint8_t* d, 16){
    //     for(auto i = 0; i < 16; i++) d[i] = i;
    // };
    // data += [](uint8_t* d){
    //     for(auto i = 0; i < 256; i++) d[i] = i;
    // };
    // data += {16, [](const std::size_t& i, uint8_t& c){ c = i } };
    ASSERT_TRUE(data() != nullptr);
}
TEST(Endian, FormatsBinaryDataAccordingToEndianType){
    auto data = test_utils::GenerateBinaryData<256>();
    auto WriteIndexValue = test_utils::WriteStrategy<16>{[](const std::size_t& i, uint8_t& c){ 
        c = i; 
    }};
    auto WriteIfMod_5 = test_utils::WriteStrategy<128>{[](const std::size_t& i, uint8_t& c){ 
        if(i % 5 == 0) c = i;
        else c = 0xFF; 
    }};
    data += WriteIndexValue;
    data += WriteIfMod_5;

    bm::BigEndian big;

    uint8_t output[256];
    big.read(data(), output, 16);
    ASSERT_EQ(output[15],0x00);
    ASSERT_EQ(output[2],0x0D);
}