// noise.c
// Procedural noise generator for FluffOS-based LPC
// Kryssa@SWmud, September 2024
// Ported and refactored from "SquirrelNoise5" by Squirrel Eiserloh

#define INT_32_UNSIGNED_MAX 0xFFFFFFFF
#define INT_32_SIGNED_MAX   0x7FFFFFFF

// Function Prototypes
int Get1dNoise(int index, int seed);
int Get2dNoise(int posX, int posY, int seed);
int Get3dNoise(int posX, int posY, int posZ, int seed);
int Get4dNoise(int posX, int posY, int posZ, int posT, int seed);

float Get1dNoiseZeroToOne(int index, int seed);
float Get2dNoiseZeroToOne(int posX, int posY, int seed);
float Get3dNoiseZeroToOne(int posX, int posY, int posZ, int seed);
float Get4dNoiseZeroToOne(int posX, int posY, int posZ, int posT, int seed);

float Get1dNoiseNegOneToOne(int index, int seed);
float Get2dNoiseNegOneToOne(int posX, int posY, int seed);
float Get3dNoiseNegOneToOne(int posX, int posY, int posZ, int seed);
float Get4dNoiseNegOneToOne(int posX, int posY, int posZ, int posT, int seed);

// Helper Functions
private int SquirrelNoise5(int positionX, int seed);
private int sanitize_seed(int seed);
private int fake_uint32_overflow(int number);
private int fake_int32_overflow(int number);
private float normalize_zero_to_one(int raw_noise);
private float normalize_neg_one_to_one(int raw_noise);

//--------------------------------------------------------------------------
/**
 * SquirrelNoise5 - Fast hash function for procedural noise.
 * Returns a 32-bit pseudorandom integer based on position and seed.
 */
private int SquirrelNoise5(int positionX, int seed) {
    // Constants - large primes with non-boring bits
    int SQ5_BIT_NOISE1 = 0xd2a80a3f; // 11010010101010000000101000111111
    int SQ5_BIT_NOISE2 = 0xa884f197; // 10101000100001001111000110010111
    int SQ5_BIT_NOISE3 = 0x6C736F4B; // 01101100011100110110111101001011
    int SQ5_BIT_NOISE4 = 0xB79F3ABB; // 10110111100111110011101010111011
    int SQ5_BIT_NOISE5 = 0x1b56c4f5; // 00011011010101101100010011110101

    int mangledBits = positionX;

    seed = sanitize_seed(seed || 0);

    mangledBits = (mangledBits * SQ5_BIT_NOISE1) & INT_32_UNSIGNED_MAX;
    mangledBits = (mangledBits + seed) & INT_32_UNSIGNED_MAX;
    mangledBits = (mangledBits ^ (mangledBits >> 9)) & INT_32_UNSIGNED_MAX;
    mangledBits = (mangledBits + SQ5_BIT_NOISE2) & INT_32_UNSIGNED_MAX;
    mangledBits = (mangledBits ^ (mangledBits >> 11)) & INT_32_UNSIGNED_MAX;
    mangledBits = (mangledBits * SQ5_BIT_NOISE3) & INT_32_UNSIGNED_MAX;
    mangledBits = (mangledBits ^ (mangledBits >> 13)) & INT_32_UNSIGNED_MAX;
    mangledBits = (mangledBits + SQ5_BIT_NOISE4) & INT_32_UNSIGNED_MAX;
    mangledBits = (mangledBits ^ (mangledBits >> 15)) & INT_32_UNSIGNED_MAX;
    mangledBits = (mangledBits * SQ5_BIT_NOISE5) & INT_32_UNSIGNED_MAX;
    mangledBits = (mangledBits ^ (mangledBits >> 17)) & INT_32_UNSIGNED_MAX;

    return mangledBits;
}

//--------------------------------------------------------------------------
/**
 * sanitize_seed - Ensures the seed is a non-negative 32-bit unsigned integer.
 */
private int sanitize_seed(int seed) {
    return abs(seed) & INT_32_UNSIGNED_MAX;
}

//--------------------------------------------------------------------------
/**
 * fake_uint32_overflow - Mimics 32-bit unsigned integer overflow.
 */
private int fake_uint32_overflow(int number) {
    return number & INT_32_UNSIGNED_MAX;
}

//--------------------------------------------------------------------------
/**
 * fake_int32_overflow - Mimics 32-bit signed integer overflow.
 */
private int fake_int32_overflow(int number) {
    if(number <= INT_32_SIGNED_MAX)
        return number;
    return (number % INT_32_SIGNED_MAX) - INT_32_SIGNED_MAX - 2;
}

//--------------------------------------------------------------------------
/**
 * normalize_zero_to_one - Normalizes raw noise to [0.0, 1.0].
 *
 * @param raw_noise: The raw noise integer value.
 * @return: Float normalized to [0.0, 1.0].
 */
private float normalize_zero_to_one(int raw_noise) {
    // Apply bitmasking to treat the signed int as unsigned
    int uResult = raw_noise & 0xFFFFFFFF;
    // Normalize to [0.0, 1.0]
    float normalized = ((float)uResult) / 4294967295.0;
    return normalized;
}

//--------------------------------------------------------------------------
/**
 * normalize_neg_one_to_one - Normalizes raw noise to [-1.0, 1.0].
 *
 * @param raw_noise: The raw noise integer value.
 * @return: Float normalized to [-1.0, 1.0].
 */
private float normalize_neg_one_to_one(int raw_noise) {
    float normalized_zero_to_one = normalize_zero_to_one(raw_noise);
    // Scale to [-1.0, 1.0]
    float normalized = (normalized_zero_to_one * 2.0) - 1.0;
    return normalized;
}

//--------------------------------------------------------------------------
/**
 * Get1dNoise - Generates 1D noise.
 */
int Get1dNoise(int positionX, int seed) {
    return SquirrelNoise5(positionX, seed);
}

//--------------------------------------------------------------------------
/**
 * Get2dNoise - Generates 2D noise by combining X and Y positions.
 */
int Get2dNoise(int posX, int posY, int seed) {
    int PRIME_NUMBER = 198491317; // Large prime number with non-boring bits
    return SquirrelNoise5(
        posX + fake_uint32_overflow(PRIME_NUMBER * posY),
        seed
    );
}

//--------------------------------------------------------------------------
/**
 * Get3dNoise - Generates 3D noise by combining X, Y, and Z positions.
 */
int Get3dNoise(int posX, int posY, int posZ, int seed) {
    int PRIME1 = 198491317; // Large prime number with non-boring bits
    int PRIME2 = 6542989;   // Large prime number with distinct, non-boring bits
    return SquirrelNoise5(
        posX + fake_uint32_overflow(PRIME1 * posY) + fake_uint32_overflow(PRIME2 * posZ),
        seed
    );
}

//--------------------------------------------------------------------------
/**
 * Get4dNoise - Generates 4D noise by combining X, Y, Z, and T positions.
 */
int Get4dNoise(int posX, int posY, int posZ, int posT, int seed) {
    int PRIME1 = 198491317; // Large prime number with non-boring bits
    int PRIME2 = 6542989;   // Large prime number with distinct, non-boring bits
    int PRIME3 = 357239;    // Large prime number with distinct, non-boring bits
    return SquirrelNoise5(
        posX +
        fake_uint32_overflow(PRIME1 * posY) +
        fake_uint32_overflow(PRIME2 * posZ) +
        fake_uint32_overflow(PRIME3 * posT),
        seed
    );
}

//--------------------------------------------------------------------------
/**
 * Get1dNoiseZeroToOne - Normalizes 1D noise to [0.0, 1.0].
 */
float Get1dNoiseZeroToOne(int index, int seed) {
    int raw_noise = Get1dNoise(index, seed);
    return normalize_zero_to_one(raw_noise);
}

//--------------------------------------------------------------------------
/**
 * Get2dNoiseZeroToOne - Normalizes 2D noise to [0.0, 1.0].
 */
float Get2dNoiseZeroToOne(int posX, int posY, int seed) {
    int raw_noise = Get2dNoise(posX, posY, seed);
    return normalize_zero_to_one(raw_noise);
}

//--------------------------------------------------------------------------
/**
 * Get3dNoiseZeroToOne - Normalizes 3D noise to [0.0, 1.0].
 */
float Get3dNoiseZeroToOne(int posX, int posY, int posZ, int seed) {
    int raw_noise = Get3dNoise(posX, posY, posZ, seed);
    return normalize_zero_to_one(raw_noise);
}

//--------------------------------------------------------------------------
/**
 * Get4dNoiseZeroToOne - Normalizes 4D noise to [0.0, 1.0].
 */
float Get4dNoiseZeroToOne(int posX, int posY, int posZ, int posT, int seed) {
    int raw_noise = Get4dNoise(posX, posY, posZ, posT, seed);
    return normalize_zero_to_one(raw_noise);
}

//--------------------------------------------------------------------------
/**
 * Get1dNoiseNegOneToOne - Normalizes 1D noise to [-1.0, 1.0].
 */
float Get1dNoiseNegOneToOne(int index, int seed) {
    int raw_noise = Get1dNoise(index, seed);
    return normalize_neg_one_to_one(raw_noise);
}

//--------------------------------------------------------------------------
/**
 * Get2dNoiseNegOneToOne - Normalizes 2D noise to [-1.0, 1.0].
 */
float Get2dNoiseNegOneToOne(int posX, int posY, int seed) {
    int raw_noise = Get2dNoise(posX, posY, seed);
    return normalize_neg_one_to_one(raw_noise);
}

//--------------------------------------------------------------------------
/**
 * Get3dNoiseNegOneToOne - Normalizes 3D noise to [-1.0, 1.0].
 */
float Get3dNoiseNegOneToOne(int posX, int posY, int posZ, int seed) {
    int raw_noise = Get3dNoise(posX, posY, posZ, seed);
    return normalize_neg_one_to_one(raw_noise);
}

//--------------------------------------------------------------------------
/**
 * Get4dNoiseNegOneToOne - Normalizes 4D noise to [-1.0, 1.0].
 */
float Get4dNoiseNegOneToOne(int posX, int posY, int posZ, int posT, int seed) {
    int raw_noise = Get4dNoise(posX, posY, posZ, posT, seed);
    return normalize_neg_one_to_one(raw_noise);
}
