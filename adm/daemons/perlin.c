// File: /adm/daemons/perlin_d.c

inherit STD_DAEMON;

#define OCTAVES 4
#define PERSISTENCE 0.5

// Function declarations
private int *init_perlin();
private float noise(float x, float y, float z);
private float fade(float t);
private float lerp(float t, float a, float b);
private float grad(int hash, float x, float y, float z);
public float perlin(float x, float y, float z);

void setup() {
    init_perlin();
}

private int *init_perlin() {
    int i, j, k;
    int *p = allocate(512);
    int *permutation = allocate(256);

    for(i = 0; i < 256; i++)
        permutation[i] = i;

    for(i = 0; i < 256; i++) {
        j = random(256 - i) + i;
        k = permutation[i];
        permutation[i] = permutation[j];
        permutation[j] = k;
    }

    for(i = 0; i < 256; i++)
        p[256 + i] = p[i] = permutation[i];

    return p;
}

float noise(float x, float y, float z) {
    int X, Y, Z;
    float u, v, w;
    int A, AA, AB, B, BA, BB;
    int *p;

    p = init_perlin();

    X = to_int(floor(x)) & 255;
    Y = to_int(floor(y)) & 255;
    Z = to_int(floor(z)) & 255;

    x -= floor(x);
    y -= floor(y);
    z -= floor(z);

    u = fade(x);
    v = fade(y);
    w = fade(z);

    A = p[X] + Y;
    AA = p[A] + Z;
    AB = p[A + 1] + Z;
    B = p[X + 1] + Y;
    BA = p[B] + Z;
    BB = p[B + 1] + Z;

    return lerp(w, lerp(v, lerp(u, grad(p[AA], x, y, z),
                                   grad(p[BA], x - 1, y, z)),
                           lerp(u, grad(p[AB], x, y - 1, z),
                                   grad(p[BB], x - 1, y - 1, z))),
                   lerp(v, lerp(u, grad(p[AA + 1], x, y, z - 1),
                                   grad(p[BA + 1], x - 1, y, z - 1)),
                           lerp(u, grad(p[AB + 1], x, y - 1, z - 1),
                                   grad(p[BB + 1], x - 1, y - 1, z - 1))));
}

private float fade(float t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

private float lerp(float t, float a, float b) {
    return a + t * (b - a);
}

private float grad(int hash, float x, float y, float z) {
    int h;
    float u, v;

    h = hash & 15;
    u = h < 8 ? x : y;
    v = h < 4 ? y : h == 12 || h == 14 ? x : z;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

float perlin(float x, float y, float z) {
    float total, frequency, amplitude, maxValue;
    int i;

    total = 0;
    frequency = 1;
    amplitude = 1;
    maxValue = 0;

    for(i = 0; i < OCTAVES; i++) {
        total += noise(x * frequency, y * frequency, z * frequency) * amplitude;
        maxValue += amplitude;
        amplitude *= PERSISTENCE;
        frequency *= 2;
    }

    return total / maxValue;
}
