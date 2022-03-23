#include <bitset>
#include <cstdint>
#include <functional>
#include <iomanip>
#include <iostream>
#include <ostream>

struct bMod
{
    explicit bMod(uint16_t b) : bitmask(b){};
    uint16_t bitmask;
};

constexpr int countTrailingZeroes(uint16_t val)
{
    return __builtin_ctz(val);
}

constexpr int countSetBits(uint16_t val)
{
    return __builtin_popcount(val);
}

constexpr bool areConsecutiveBitsSet(uint16_t val)
{
    uint16_t maskShifted = val >> countTrailingZeroes(val);
    uint16_t mask = (1 << countSetBits(val)) - 1;

    return !(mask ^ maskShifted);
}

constexpr uint16_t getMaskedValue(uint16_t value, uint16_t bitmask)
{
    uint16_t maskShifted = bitmask >> countTrailingZeroes(bitmask);
    uint16_t valueShifted = value >> countTrailingZeroes(bitmask);

    return valueShifted & maskShifted;
}

constexpr uint16_t reverseBitmask(uint16_t value)
{
    // 0b0000, 0b0001, 0b0010, 0b0011, 0b0100, 0b0101, 0b0110, 0b0111,
    // 0b1000, 0b1001, 0b1010, 0b1011, 0b1100, 0b1101, 0b1110, 0b1111
    // swap bits in pairs
    value = (((value & 0xaaaa) >> 1) | ((value & 0x5555) << 1));
    // swap pairs of bits in nibble
    value = (((value & 0xcccc) >> 2) | ((value & 0x3333) << 2));
    // swap nibbles in bytes
    value = (((value & 0xf0f0) >> 4) | ((value & 0x0f0f) << 4));

    // swap bytes
    return ((value & 0xff00) >> 8) | ((value & 0x00ff) << 8);
}

constexpr uint32_t reverseBitmask_32(uint32_t value)
{
    // swap half words
    return reverseBitmask((value >> 16) & 0xffff) | reverseBitmask(value & 0xffff) << 16;
}

constexpr uint64_t reverseBitmask_64(uint64_t value)
{
    // swap words
    return reverseBitmask_32((value >> 32) & 0xffffffff) |
           ((uint64_t)reverseBitmask_32(value & 0xffffffff)) << 32;
}

std::ostream& printBitmask(std::ostream &os, uint16_t bitmask)
{
    uint16_t maskShifted = bitmask >> countTrailingZeroes(bitmask);

    uint16_t mask1 = (1 << countSetBits(bitmask)) - 1;
    uint16_t count = countSetBits(bitmask);
    std::bitset<16> b(bitmask);
    return os << std::hex << b
              << " ctz: " << countTrailingZeroes(bitmask)
              << " count: " << std::hex << count

              << " mask : " << std::hex << std::bitset<16>(maskShifted)

              << " mask1: " << std::hex << std::bitset<16>(mask1)

              << " isConsecutive: " << std::hex << areConsecutiveBitsSet(bitmask)
            ;
}

std::ostream& operator<<(std::ostream& os, bMod m)
{
    return printBitmask(os, m.bitmask);
}

int main(int, char **)
{
    uint16_t duPortBitmask = 0b1111000000000000;
    uint16_t bandSectorBitmask = 0b0000111100000000;
    uint16_t ccIdBitmask = 0b0000000011110000;
    uint16_t ruPortBitmask = 0b0000000000001111;
    const uint64_t eaxcIdBitmask = 0b1111111111111111;

    uint16_t orBitmasks = duPortBitmask | bandSectorBitmask | ccIdBitmask | ruPortBitmask;
    uint16_t andBitmasks = duPortBitmask & bandSectorBitmask & ccIdBitmask & ruPortBitmask;
    uint16_t xorBitmasks = duPortBitmask ^ bandSectorBitmask ^ ccIdBitmask ^ ruPortBitmask;

    std::cout << std::hex << "duPortBitmask:" << bMod{duPortBitmask} << std::endl
              << "bandSectorBitmask: " << bMod{bandSectorBitmask} << std::endl
              << "ccIdBitmask: " << bMod{ccIdBitmask} << std::endl
              << "ruPortBitmask: " << bMod{ruPortBitmask} << std::endl
              << "orBitmasks: " << bMod{orBitmasks} << std::endl
              << "andBitmasks: " << bMod{andBitmasks} << std::endl
              << "xorBitmasks: " << bMod(xorBitmasks) << std::endl
              << "xor: " << bMod(orBitmasks ^ eaxcIdBitmask) << std::endl
              << "and: " << bMod(orBitmasks & eaxcIdBitmask) << std::endl
              << "0x70: " << bMod(0x70) << std::endl
              << "0x50: " << bMod(0x50) << std::endl
              << "0x700: " << bMod(0x700) << std::endl
              << "0x500: " << bMod(0x500) << std::endl
              << "0x1000: " << bMod(0x1000) << std::endl
              << "0x2000: " << bMod(0x2000) << std::endl
              << "0x3000: " << bMod(0x3000) << std::endl
              << "0x4000: " << bMod(0x4000) << std::endl
              << "0x5000: " << bMod(0x5000) << std::endl
              << "0x6000: " << bMod(0x6000) << std::endl
              << "0x7000: " << bMod(0x7000) << std::endl
              << "0x8000: " << bMod(0x8000) << std::endl
              << "0x9000: " << bMod(0x9000) << std::endl
              << "0xa000: " << bMod(0xa000) << std::endl;

    uint16_t eaxcId = 0b0000000011101010;
    std::cout << std::hex << "eaxcId: " << eaxcId
              << " duPort: " << getMaskedValue(eaxcId, 0b1100000)
              << " bandSector: " << getMaskedValue(eaxcId, 0b110000)
              << " ccid: " << getMaskedValue(eaxcId, 0b1100)
              << " ruPort: " << getMaskedValue(eaxcId, 0b11) << std::endl;

    std::cout << "0b1100001101011010: " << bMod(reverseBitmask(0b1100001101011010))
              << std::endl
              << "0b0011110011000011: " << bMod(reverseBitmask(0b0011110011000011))
              << std::endl;

    std::cout << "0b11000011010110100011110011000011: "
              << std::bitset<32>(reverseBitmask_32(0b11000011010110100011110011000011)) << std::endl
              << "0b00111100110000111100001101011010: "
              << std::bitset<32>(reverseBitmask_32(0b00111100110000111100001101011010)) << std::endl;

    std::cout << std::setfill('=') << std::left << std::setw(80) << " 64 bit integer reverse "
              << std::endl
              << "0b1100001101011010001111001100001100111100110000111100001101011010: "
              << std::bitset<64>(reverseBitmask_64(
                     0b1100001101011010001111001100001100111100110000111100001101011010))
              << std::endl
              << "0b0011110011000011110000110101101011000011010110100011110011000011: "
              << std::bitset<64>(reverseBitmask_64(
                     0b0011110011000011110000110101101011000011010110100011110011000011))
              << std::endl;
}
