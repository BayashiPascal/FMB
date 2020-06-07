
camera {
  location 0.21 * (z + y)
  look_at 0.09 * y + 0.025 * x
}

light_source {
  2.0
  color rgb 1.0
}

background { color rgb 1.0 }

#include "bunny.inc"
#include "fmb.inc"

union {
  union {
    object {
      bunny
      scale 1/1000
    }
    object {
      bunny
      scale 1/1000
      scale <-1,1,1>
      translate 0.05 * x - 0.01 * y
    }
    texture { 
      pigment { color rgbt <1.0, 1.0, 1.0, 0.9> } 
      finish { phong 0.05 phong_size 10 }
    }
  }
  object {
    fmb
    scale 1/1000
    texture { 
      pigment { color rgb x }
    }
  }
  rotate y * (45 + 360 * clock)
}
