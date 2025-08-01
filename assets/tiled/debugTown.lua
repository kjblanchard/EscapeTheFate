return {
  version = "1.5",
  luaversion = "5.1",
  tiledversion = "1.8.6",
  orientation = "orthogonal",
  renderorder = "right-down",
  width = 38,
  height = 18,
  tilewidth = 16,
  tileheight = 16,
  nextlayerid = 7,
  nextobjectid = 48,
  properties = {
    ["bgm"] = "town2",
    ["display"] = "Debug Town"
  },
  tilesets = {
    {
      name = "terrain",
      firstgid = 1,
      tilewidth = 16,
      tileheight = 16,
      spacing = 0,
      margin = 0,
      columns = 39,
      image = "../img/terrain-export.bmp",
      imagewidth = 624,
      imageheight = 608,
      transparentcolor = "#ff00ff",
      objectalignment = "unspecified",
      tileoffset = {
        x = 0,
        y = 0
      },
      grid = {
        orientation = "orthogonal",
        width = 16,
        height = 16
      },
      properties = {},
      wangsets = {},
      tilecount = 1482,
      tiles = {
        {
          id = 165,
          animation = {
            {
              tileid = 165,
              duration = 200
            },
            {
              tileid = 166,
              duration = 200
            },
            {
              tileid = 167,
              duration = 200
            }
          }
        }
      }
    },
    {
      name = "outside",
      firstgid = 1483,
      tilewidth = 16,
      tileheight = 16,
      spacing = 0,
      margin = 0,
      columns = 52,
      image = "../img/outside-export.bmp",
      imagewidth = 832,
      imageheight = 384,
      transparentcolor = "#ff00ff",
      objectalignment = "unspecified",
      tileoffset = {
        x = 0,
        y = 0
      },
      grid = {
        orientation = "orthogonal",
        width = 16,
        height = 16
      },
      properties = {},
      wangsets = {},
      tilecount = 1248,
      tiles = {}
    },
    {
      name = "house",
      firstgid = 2731,
      tilewidth = 16,
      tileheight = 16,
      spacing = 0,
      margin = 0,
      columns = 69,
      image = "../img/house-export.bmp",
      imagewidth = 1104,
      imageheight = 608,
      transparentcolor = "#ff00ff",
      objectalignment = "unspecified",
      tileoffset = {
        x = 0,
        y = 0
      },
      grid = {
        orientation = "orthogonal",
        width = 16,
        height = 16
      },
      properties = {},
      wangsets = {},
      tilecount = 2622,
      tiles = {}
    }
  },
  layers = {
    {
      type = "group",
      id = 6,
      name = "bg1",
      visible = true,
      opacity = 1,
      offsetx = 0,
      offsety = 0,
      parallaxx = 1,
      parallaxy = 1,
      properties = {},
      layers = {
        {
          type = "tilelayer",
          x = 0,
          y = 0,
          width = 38,
          height = 18,
          id = 1,
          name = "Tile Layer 1",
          visible = true,
          opacity = 1,
          offsetx = 0,
          offsety = 0,
          parallaxx = 1,
          parallaxy = 1,
          properties = {},
          encoding = "lua",
          data = {
            42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 48, 48, 48, 48, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42,
            42, 120, 42, 42, 42, 42, 42, 42, 42, 42, 48, 48, 48, 48, 48, 48, 42, 42, 129, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42,
            42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 48, 48, 48, 48, 48, 48, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42,
            42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 48, 48, 42, 42, 42, 42, 48, 48, 48, 48, 48, 48, 42, 42, 93, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42,
            42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 48, 48, 42, 42, 42, 42, 48, 42, 42, 42, 48, 48, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42,
            42, 119, 42, 42, 42, 42, 42, 42, 42, 42, 48, 48, 42, 42, 42, 42, 48, 42, 42, 42, 48, 48, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42,
            42, 42, 42, 42, 42, 42, 47, 47, 47, 48, 48, 48, 42, 42, 42, 48, 48, 42, 42, 42, 42, 48, 48, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42,
            42, 42, 42, 42, 42, 42, 47, 47, 47, 42, 42, 42, 42, 42, 42, 47, 48, 42, 42, 42, 42, 48, 48, 42, 42, 42, 42, 165, 165, 165, 42, 42, 42, 42, 42, 42, 42, 42,
            42, 42, 42, 42, 42, 42, 47, 47, 47, 42, 42, 42, 42, 42, 42, 48, 48, 42, 42, 42, 47, 48, 48, 42, 42, 42, 42, 47, 47, 47, 42, 42, 42, 42, 42, 42, 42, 42,
            42, 42, 42, 42, 42, 42, 47, 47, 47, 42, 42, 123, 42, 42, 42, 48, 48, 42, 48, 48, 48, 48, 47, 42, 42, 42, 42, 47, 47, 47, 42, 42, 42, 42, 42, 42, 42, 42,
            42, 42, 42, 42, 42, 42, 47, 47, 47, 42, 42, 42, 42, 42, 42, 48, 47, 47, 48, 48, 48, 48, 48, 42, 42, 42, 42, 47, 47, 47, 42, 42, 42, 42, 42, 42, 42, 42,
            42, 42, 42, 42, 119, 42, 47, 47, 47, 42, 42, 42, 42, 42, 42, 48, 47, 47, 48, 48, 48, 48, 48, 42, 42, 42, 42, 47, 47, 47, 42, 42, 42, 42, 42, 42, 42, 42,
            42, 42, 42, 42, 42, 42, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 42, 42, 42, 42, 42, 42, 42, 42,
            93, 42, 42, 42, 42, 42, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 42, 42, 42, 42, 42, 42, 42, 42,
            42, 42, 42, 42, 42, 42, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 42, 42, 42, 42, 42, 42, 42, 42,
            42, 42, 42, 47, 47, 47, 47, 47, 47, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42,
            42, 42, 42, 47, 47, 47, 47, 47, 47, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42,
            119, 42, 42, 165, 165, 165, 42, 42, 42, 42, 42, 129, 42, 42, 42, 42, 42, 93, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 93, 42, 42, 42, 42, 42, 42
          }
        },
        {
          type = "tilelayer",
          x = 0,
          y = 0,
          width = 38,
          height = 18,
          id = 2,
          name = "Tile Layer 2",
          visible = true,
          opacity = 1,
          offsetx = 0,
          offsety = 0,
          parallaxx = 1,
          parallaxy = 1,
          properties = {},
          encoding = "lua",
          data = {
            0, 0, 1864, 1865, 1866, 0, 0, 0, 0, 0, 0, 0, 1759, 1560, 1561, 1759, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1644, 1649, 1649, 1649, 1649, 1649, 1644, 1693, 1694, 1694, 1694, 1694, 1697,
            0, 0, 1916, 1917, 1918, 1759, 0, 0, 0, 0, 0, 0, 1759, 1612, 1613, 1759, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1644, 0, 0, 0, 0, 0, 1644, 1744, 166, 166, 166, 166, 1744,
            0, 0, 1968, 1969, 1970, 166, 2108, 2109, 2110, 0, 0, 0, 1759, 1759, 1759, 1759, 166, 0, 0, 0, 0, 166, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1744, 166, 166, 166, 166, 1744,
            0, 0, 2020, 2021, 2022, 2159, 2160, 2161, 2162, 0, 0, 0, 0, 0, 0, 0, 1693, 1694, 1694, 1694, 1694, 1697, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1744, 166, 166, 166, 166, 1744,
            166, 0, 0, 0, 0, 2211, 2212, 2213, 2214, 0, 0, 0, 0, 0, 0, 0, 1692, 124, 0, 0, 0, 1744, 0, 0, 1542, 0, 0, 0, 0, 0, 0, 0, 1744, 166, 166, 166, 166, 1744,
            1759, 0, 1759, 0, 0, 2263, 2264, 2265, 2266, 0, 0, 0, 1864, 1865, 1866, 0, 1692, 0, 0, 0, 0, 1744, 0, 0, 1594, 0, 0, 0, 0, 0, 0, 0, 1744, 166, 166, 166, 166, 1744,
            0, 0, 0, 2316, 2317, 2318, 0, 0, 0, 0, 0, 0, 1916, 1917, 1918, 0, 1692, 0, 0, 0, 0, 1744, 0, 0, 1594, 0, 0, 0, 0, 0, 0, 0, 1745, 1746, 1797, 1746, 1746, 1749,
            0, 0, 1759, 2368, 2369, 2370, 0, 0, 0, 166, 0, 0, 1968, 1969, 1970, 0, 1692, 0, 0, 0, 0, 1744, 0, 0, 166, 1910, 1692, 0, 0, 0, 1692, 1548, 166, 0, 0, 0, 0, 0,
            0, 0, 0, 2420, 2421, 2422, 0, 0, 0, 0, 0, 0, 2020, 2021, 2022, 0, 1692, 126, 0, 0, 126, 1744, 0, 0, 0, 0, 1744, 0, 0, 0, 1744, 1600, 0, 0, 0, 0, 0, 0,
            0, 1759, 0, 2472, 2473, 2474, 0, 0, 0, 1759, 0, 0, 0, 0, 1759, 0, 1693, 1797, 0, 0, 1693, 1749, 0, 1759, 0, 0, 1796, 0, 0, 0, 1796, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 166, 0, 0, 0, 0, 166, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1759, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 1759, 0, 1588, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1656, 1657, 1658, 1659, 1656, 1657, 1658, 1659, 1656, 1657, 1658, 1659,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1759, 0, 0, 0, 0, 0, 0, 1756, 0, 0, 0, 0, 0, 0, 0, 1708, 1709, 1710, 1711, 1708, 1709, 1710, 1711, 1708, 1709, 1710, 1711,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1756, 0, 0, 0, 0, 0, 0, 0, 0, 1553, 1554, 1555, 0, 0, 1760, 1761, 1762, 1763, 1760, 1761, 1762, 1763, 1760, 1761, 1762, 1763,
            0, 0, 166, 0, 0, 0, 166, 0, 0, 1756, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1756, 1605, 1606, 1607, 0, 0, 1812, 1813, 1814, 1815, 1812, 1813, 1814, 1815, 1812, 1813, 1814, 1815
          }
        },
        {
          type = "tilelayer",
          x = 0,
          y = 0,
          width = 38,
          height = 18,
          id = 4,
          name = "Tile Layer 3",
          visible = true,
          opacity = 1,
          offsetx = 0,
          offsety = 0,
          parallaxx = 1,
          parallaxy = 1,
          properties = {},
          encoding = "lua",
          data = {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3370, 3371, 3372, 3373, 3374, 3375, 3376, 3377, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3439, 3440, 3441, 3442, 3443, 3444, 3445, 3446, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3508, 3509, 3510, 3511, 3512, 3513, 3514, 3515, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1990, 1991, 1992, 1993, 1994, 1995, 0, 0, 3577, 3578, 3579, 3580, 3581, 3582, 3583, 3584, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2042, 2043, 2044, 2045, 2046, 2047, 0, 0, 3646, 3647, 3648, 3649, 3650, 3651, 3652, 3653, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2094, 2095, 2096, 2097, 2098, 2099, 0, 0, 1542, 3422, 3423, 3425, 3426, 3427, 3423, 3424, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2146, 2147, 2148, 2149, 2150, 2151, 0, 0, 1594, 3491, 3492, 3494, 3495, 3496, 3492, 3493, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2198, 2199, 2200, 2201, 2202, 2203, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2250, 2251, 2252, 2253, 2254, 2255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2302, 2303, 2304, 2305, 2306, 2307, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
          }
        }
      }
    },
    {
      type = "objectgroup",
      draworder = "topdown",
      id = 3,
      name = "solid",
      visible = false,
      opacity = 1,
      offsetx = 0,
      offsety = 0,
      parallaxx = 1,
      parallaxy = 1,
      properties = {},
      objects = {
        {
          id = 3,
          name = "",
          type = "",
          shape = "rectangle",
          x = 261,
          y = 48,
          width = 85,
          height = 112,
          rotation = 0,
          visible = true,
          properties = {}
        },
        {
          id = 4,
          name = "",
          type = "",
          shape = "rectangle",
          x = 401.5,
          y = 0,
          width = 110.5,
          height = 112,
          rotation = 0,
          visible = true,
          properties = {}
        },
        {
          id = 15,
          name = "",
          type = "",
          shape = "rectangle",
          x = 421.364,
          y = 112,
          width = 5.63636,
          height = 48,
          rotation = 0,
          visible = true,
          properties = {}
        },
        {
          id = 16,
          name = "",
          type = "",
          shape = "rectangle",
          x = 485,
          y = 112,
          width = 6.63636,
          height = 48,
          rotation = 0,
          visible = true,
          properties = {}
        },
        {
          id = 17,
          name = "",
          type = "",
          shape = "rectangle",
          x = 496,
          y = 112,
          width = 14.75,
          height = 26,
          rotation = 0,
          visible = true,
          properties = {}
        },
        {
          id = 18,
          name = "",
          type = "",
          shape = "rectangle",
          x = 384,
          y = 73,
          width = 16,
          height = 38.5455,
          rotation = 0,
          visible = true,
          properties = {}
        },
        {
          id = 20,
          name = "",
          type = "",
          shape = "rectangle",
          x = 199.25,
          y = 92.25,
          width = 29.9621,
          height = 46.3409,
          rotation = 0,
          visible = true,
          properties = {}
        },
        {
          id = 21,
          name = "",
          type = "",
          shape = "rectangle",
          x = 96,
          y = 48,
          width = 48,
          height = 48,
          rotation = 0,
          visible = true,
          properties = {}
        },
        {
          id = 22,
          name = "",
          type = "",
          shape = "rectangle",
          x = 39.5,
          y = 10,
          width = 28.5,
          height = 48,
          rotation = 0,
          visible = true,
          properties = {}
        },
        {
          id = 23,
          name = "",
          type = "",
          shape = "rectangle",
          x = 53,
          y = 107,
          width = 37,
          height = 48,
          rotation = 0,
          visible = true,
          properties = {}
        },
        {
          id = 24,
          name = "",
          type = "",
          shape = "rectangle",
          x = 48,
          y = 224,
          width = 16,
          height = 16,
          rotation = 0,
          visible = true,
          properties = {}
        },
        {
          id = 25,
          name = "",
          type = "",
          shape = "rectangle",
          x = 214,
          y = 6.5,
          width = 22,
          height = 20.75,
          rotation = 0,
          visible = true,
          properties = {}
        },
        {
          id = 26,
          name = "",
          type = "",
          shape = "rectangle",
          x = 341.25,
          y = 261.5,
          width = 40.5,
          height = 19.25,
          rotation = 0,
          visible = true,
          properties = {}
        },
        {
          id = 34,
          name = "",
          type = "",
          shape = "rectangle",
          x = 512,
          y = 0,
          width = 96,
          height = 112,
          rotation = 0,
          visible = true,
          properties = {}
        },
        {
          id = 43,
          name = "",
          type = "",
          shape = "rectangle",
          x = 400,
          y = 112,
          width = 16,
          height = 16,
          rotation = 0,
          visible = true,
          properties = {}
        },
        {
          id = 44,
          name = "",
          type = "",
          shape = "rectangle",
          x = 424.25,
          y = 224.5,
          width = 175.25,
          height = 61.325,
          rotation = 0,
          visible = true,
          properties = {}
        }
      }
    },
    {
      type = "objectgroup",
      draworder = "topdown",
      id = 5,
      name = "entities",
      visible = true,
      opacity = 1,
      offsetx = 0,
      offsety = 0,
      parallaxx = 1,
      parallaxy = 1,
      properties = {},
      objects = {
        {
          id = 6,
          name = "Exit0",
          type = "2Exit",
          shape = "rectangle",
          x = 50.7273,
          y = 279,
          width = 41.4545,
          height = 9,
          rotation = 0,
          visible = true,
          properties = {
            ["loadLocation"] = 1,
            ["loadMap"] = "debugSouth"
          }
        },
        {
          id = 7,
          name = "Start0",
          type = "4Start",
          shape = "rectangle",
          x = 112,
          y = 96,
          width = 16,
          height = 16,
          rotation = 0,
          visible = true,
          properties = {
            ["direction"] = 1,
            ["loadLocation"] = 0
          }
        },
        {
          id = 10,
          name = "Start1",
          type = "4Start",
          shape = "rectangle",
          x = 64.1667,
          y = 242.333,
          width = 16,
          height = 16,
          rotation = 0,
          visible = true,
          properties = {
            ["direction"] = 0,
            ["loadLocation"] = 1
          }
        },
        {
          id = 13,
          name = "House Exit",
          type = "2Exit",
          shape = "rectangle",
          x = 432,
          y = 112,
          width = 48,
          height = 9,
          rotation = 0,
          visible = true,
          properties = {
            ["loadLocation"] = 1,
            ["loadMap"] = "debugTownHome"
          }
        },
        {
          id = 14,
          name = "Start2",
          type = "4Start",
          shape = "rectangle",
          x = 448.333,
          y = 125.667,
          width = 16,
          height = 16,
          rotation = 0,
          visible = true,
          properties = {
            ["direction"] = 2,
            ["loadLocation"] = 2
          }
        },
        {
          id = 30,
          name = "Sign",
          type = "5TextInteract",
          shape = "rectangle",
          x = 272,
          y = 144,
          width = 16,
          height = 16,
          rotation = 0,
          visible = true,
          properties = {
            ["filename"] = "treesign"
          }
        },
        {
          id = 31,
          name = "ForestSign",
          type = "5TextInteract",
          shape = "rectangle",
          x = 48,
          y = 224,
          width = 16,
          height = 14.9832,
          rotation = 0,
          visible = true,
          properties = {
            ["filename"] = "sign1"
          }
        },
        {
          id = 32,
          name = "Sign",
          type = "5TextInteract",
          shape = "rectangle",
          x = 400,
          y = 112,
          width = 16,
          height = 16,
          rotation = 0,
          visible = true,
          properties = {
            ["filename"] = "home"
          }
        },
        {
          id = 33,
          name = "Sign",
          type = "5TextInteract",
          shape = "rectangle",
          x = 48,
          y = 112,
          width = 43.3117,
          height = 45.9129,
          rotation = 0,
          visible = true,
          properties = {
            ["filename"] = "fire"
          }
        },
        {
          id = 37,
          name = "Sign",
          type = "5TextInteract",
          shape = "rectangle",
          x = 528,
          y = 96,
          width = 48,
          height = 16,
          rotation = 0,
          visible = true,
          properties = {
            ["filename"] = "flowersign"
          }
        },
        {
          id = 46,
          name = "player?",
          type = "1BattleZone",
          shape = "rectangle",
          x = 352,
          y = 208,
          width = 32,
          height = 32,
          rotation = 0,
          visible = true,
          properties = {
            ["battleId"] = "0",
            ["encounterTime"] = 5
          }
        }
      }
    }
  }
}
