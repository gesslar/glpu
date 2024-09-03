private nosave int *p; // Ensure p is declared at the top
private nosave int *perm;
private nosave mixed *gradP;
protected nosave int *grid_size = ({ 10, 10 });

float F2, G2;

mixed *grad3;

void seed(int seed);

void init_noise(int x, int y, int seed) {
  grad3 = ({
      ({1,1,0}), ({-1,1,0}), ({1,-1,0}), ({-1,-1,0}),
      ({1,0,1}), ({-1,0,1}), ({1,0,-1}), ({-1,0,-1}),
      ({0,1,1}), ({0,-1,1}), ({0,1,-1}), ({0,-1,-1})
  });

  // Ensure grid_size is set correctly
  grid_size = ({ x, y });

  // Allocate p, perm, and gradP based on grid size
  p = allocate(grid_size[0] * 2); // For p array
  perm = allocate(grid_size[0] * 2); // For permutation table
  gradP = allocate(grid_size[0] * grid_size[1] * 2); // Adjusted for gradient values

  // Initialize p array with values from 0 to 255
  for(int i = 0; i < grid_size[0] * 2; i++) {
    p[i] = i; // Ensure p is filled correctly
  }

  if(seed > 0) {
    shuffle(p) ;
  }

  // Shuffle the p array
  for(int i = grid_size[0] * 2 - 1; i > 0; i--) {
    int j = random(i + 1); // Get a random index
    // Swap p[i] with p[j]
    int temp = p[i];
    p[i] = p[j];
    p[j] = temp;
  }

  printf("Size of perm = %d, size of gradP = %d\n", sizeof(perm), sizeof(gradP));

  // Initialize perm and gradP
  for(int i = 0; i < grid_size[0]; i++) {
    perm[i] = p[i % (grid_size[0] * 2)]; // Use modulo to wrap around
    perm[i + grid_size[0]] = perm[i]; // Duplicate for wrapping
    gradP[i] = grad3[perm[i] % 12]; // Initialize gradient values
    gradP[i + grid_size[0]] = grad3[perm[i] % 12]; // Use correct wrapping for height
  }

  // Print grid size for debugging
  // printf("grid_size[0] = %d, grid_size[1] = %d\n", grid_size[0], grid_size[1]);
  // printf("size of perm = %d, size of gradP = %d\n", sizeof(perm), sizeof(gradP));

  F2 = 0.5 * (sqrt(3.0) - 1.0);
  G2 = (3.0 - sqrt(3.0)) / 6.0;

  seed(seed);
}

void seed(int seed) {
  int i;

  if(seed > 0 && seed < 1) {
    // Scale the seed out
    seed *= 65536;
  }

  // Update the condition to use grid_size
  if(seed < grid_size[0]) {
    seed |= seed << 8;
  }

  // Ensure that perm and gradP are allocated correctly
  // Assuming you have allocated perm and gradP with size grid_size[0] * 2
  for(i = 0; i < grid_size[0]; i++) {
    int v;
    if(i & 1) {
      v = p[i] ^ (seed & (grid_size[0] - 1)); // Use grid_size[0] - 1 for masking
    } else {
      v = p[i] ^ ((seed >> 8) & (grid_size[0] - 1)); // Use grid_size[0] - 1 for masking
    }

    // Ensure you do not exceed the bounds of perm
    if(i < grid_size[0]) {
      perm[i] = v; // Assign value to perm
      perm[i + grid_size[0]] = v; // This should be safe if perm is allocated correctly
    }

    // Ensure you do not exceed the bounds of gradP
    if(i < grid_size[0]) {
      gradP[i] = grad3[v % 12]; // Adjust for gradient
      gradP[i + grid_size[0]] = grad3[v % 12]; // This should be safe if gradP is allocated correctly
    }
  }
}

float dot2(int *grad, float x, float y) {
  float result ;

  // Debugging: Print the gradient and coordinates
  // printf("dot2 called with grad = {%d, %d}, x = %f, y = %f\n", grad[0], grad[1], x, y);

  result = to_float(grad[0]) * x + to_float(grad[1]) * y;

  // Debugging: Print the result of the dot product
  // printf("dot2 result = %f\n", result);

  return result;
}

float simplex2(int xin, int yin) {
  float n0, n1, n2; // Noise contributions from the three corners
  // Skew the input space to determine which simplex cell we're in

  float s, t;
  int i, j, i1, j1;
  float x0, y0, x1, y1, x2, y2;
  int *gi0, *gi1, *gi2;
  float t0;
  float t1, t2;

  s = (to_float(xin) + to_float(yin)) * F2; // Hairy factor for 2D
  i = to_int(floor( to_float(xin) + s ));
  j = to_int(floor( to_float(yin) + s ));
  t = to_float(i+j) * G2;
  x0 = to_float(xin) - to_float(i) + t; // The x,y distances from the cell origin, unskewed.
  y0 = to_float(yin) - to_float(j) + t;
  // For the 2D case, the simplex shape is an equilateral triangle.
  // Determine which simplex we are in.

  if(x0>y0) { // lower triangle, XY order: (0,0)->(1,0)->(1,1)
    i1=1; j1=0;
  } else {    // upper triangle, YX order: (0,0)->(0,1)->(1,1)
    i1=0; j1=1;
  }
  // A step of (1,0) in (i,j) means a step of (1-c,-c) in (x,y), and
  // a step of (0,1) in (i,j) means a step of (-c,1-c) in (x,y), where
  // c = (3-sqrt(3))/6
  x1 = x0 - (float)i1 + G2; // Offsets for middle corner in (x,y) unskewed coords
  y1 = y0 - (float)j1 + G2;
  x2 = x0 - 1.0 + 2.0 * G2; // Offsets for last corner in (x,y) unskewed coords
  y2 = y0 - 1.0 + 2.0 * G2;
  // Work out the hashed gradient indices of the three simplex corners
  i &= 255;
  j &= 255;
  gi0 = gradP[i+perm[j]];
  gi1 = gradP[i+i1+perm[j+j1]];
  gi2 = gradP[i+1+perm[j+1]];
  // Calculate the contribution from the three corners
  t0 = 0.5 - x0 * x0 - y0 * y0;

  if(t0 < 0.0) {
    n0 = 0.0;
  } else {
    t0 *= t0;
    n0 = t0 * t0 * dot2(gi0, (float)x0, (float)y0);  // (x,y) of grad3 used for 2D gradient
  }

  t1 = 0.5 - x1*x1-y1*y1;

  if(t1 < 0.0) {
    n1 = 0.0;
  } else {
    t1 *= t1;
    n1 = t1 * t1 * dot2(gi1, (float)x1, (float)y1);
  }

  t2 = 0.5 - x2*x2 - y2*y2;
  if(t2 < 0.0) {
    n2 = 0.0;
  } else {
    t2 *= t2;
    n2 = t2 * t2 * dot2(gi2, (float)x2, (float)y2);
  }
  // Add contributions from each corner to get the final noise value.
  // The result is scaled to return values in the interval [-1,1].
  return 70.0 * (n0 + n1 + n2);
}


float fade(float t) {
  return t*t*t*(t*(t*6.0-15.0)+10.0);
}

float lerp(float a, float b, float t) {
  return (1.0 - t) * a + t * b;
}

// 2D Perlin Noise
float perlin2 (float x, float y) {
  int X, Y;
  float n00, n01, n10, n11;
  float u;
  float relX, relY;
  float result;
  float offsetX1, offsetY1, offsetX2, offsetY2; // Declare offsets at the top

  printf("x = %f, y = %f\n", x, y);

  X = to_int(floor(x));
  Y = to_int(floor(y));
  printf("X = %d, Y = %d\n", X, Y);

  // Get relative xy coordinates of point within that cell
  relX = x - to_float(X); // Calculate relative x
  relY = y - to_float(Y); // Calculate relative y
  printf("relX = %f, relY = %f\n", relX, relY);

  // Wrap the integer cells based on grid size
  X = X % grid_size[0]; // Update wrapping for width
  Y = Y % grid_size[1]; // Update wrapping for height

  printf("All values\n"
      "relX = %f, relY = %f, X = %d, Y = %d\n", relX, relY, X, Y);

  printf("grid_size[0] = %d, grid_size[1] = %d\n", grid_size[0], grid_size[1]);

  printf("perm[Y] = %d, perm[Y + 1] = %d\n", perm[Y], perm[Y + 1]);

  // Add constrained random offsets to the corners
  offsetX1 = random_float(0.1) - 0.05; // Random float in [-0.05, 0.05]
  offsetY1 = random_float(0.1) - 0.05; // Random float in [-0.05, 0.05]
  offsetX2 = random_float(0.1) - 0.05; // Random float in [-0.05, 0.05]
  offsetY2 = random_float(0.1) - 0.05; // Random float in [-0.05, 0.05]

  // Calculate noise contributions from each of the four corners with offsets
  n00 = dot2(gradP[X + perm[Y]], relX + offsetX1, relY + offsetY1);
  n01 = dot2(gradP[X + perm[(Y + 1) % grid_size[1]]], relX + offsetX2, relY - 1.0 + offsetY2);
  n10 = dot2(gradP[(X + 1) % grid_size[0] + perm[Y]], relX - 1.0 + offsetX1, relY + offsetY1);
  n11 = dot2(gradP[(X + 1) % grid_size[0] + perm[(Y + 1) % grid_size[1]]], relX - 1.0 + offsetX2, relY - 1.0 + offsetY2);

  // Debugging: Print the gradient values being used
  // printf("gradP[X + perm[Y]] = gradP[%d] = %O\n", X + perm[Y], gradP[X + perm[Y]]);
  // printf("gradP[X + perm[(Y + 1) %% grid_size[1]]] = gradP[%d] = %O\n", X + perm[(Y + 1) % grid_size[1]], gradP[X + perm[(Y + 1) % grid_size[1]]]);
  // printf("gradP[(X + 1) %% grid_size[0] + perm[Y]] = gradP[%d] = %O\n", (X + 1) % grid_size[0] + perm[Y], gradP[(X + 1) % grid_size[0] + perm[Y]]);
  // printf("gradP[(X + 1) %% grid_size[0] + perm[(Y + 1) %% grid_size[1]]] = gradP[%d] = %O\n", (X + 1) % grid_size[0] + perm[(Y + 1) % grid_size[1]], gradP[(X + 1) % grid_size[0] + perm[(Y + 1) % grid_size[1]]]);

  // printf("n00 = %f, n01 = %f, n10 = %f, n11 = %f\n", n00, n01, n10, n11);

  // Compute the fade curve value for relX
  u = fade(relX);

  printf("u = %f\n", u);

  // Interpolate the four results
  result = lerp(
          lerp(n00, n10, u),
          lerp(n01, n11, u),
          fade(relY));

  printf("result = %f\n", result);

  return result;
}
