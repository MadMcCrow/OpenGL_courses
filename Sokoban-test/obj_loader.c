#include <assert.h>
#include "obj_loader.h"
#define TINYOBJ_LOADER_C_IMPLEMENTATION
#include "inc/tinyobj_loader_c/tinyobj_loader_c.h"
// this is from tinyobj_loader_c/exemple to work with this "game"


static void CalcNormal(float N[3], float v0[3], float v1[3], float v2[3]) {
  float v10[3];
  float v20[3];
  float len2;

  v10[0] = v1[0] - v0[0];
  v10[1] = v1[1] - v0[1];
  v10[2] = v1[2] - v0[2];

  v20[0] = v2[0] - v0[0];
  v20[1] = v2[1] - v0[1];
  v20[2] = v2[2] - v0[2];

  N[0] = v20[1] * v10[2] - v20[2] * v10[1];
  N[1] = v20[2] * v10[0] - v20[0] * v10[2];
  N[2] = v20[0] * v10[1] - v20[1] * v10[0];

  len2 = N[0] * N[0] + N[1] * N[1] + N[2] * N[2];
  if (len2 > 0.0f) {
    float len = (float)sqrt((double)len2);

    N[0] /= len;
    N[1] /= len;
  }
}




static const char* mmap_file(size_t* len, const char* filename) {
#ifdef _WIN64
  HANDLE file =
      CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
                  FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
  assert(file != INVALID_HANDLE_VALUE);

  HANDLE fileMapping = CreateFileMapping(file, NULL, PAGE_READONLY, 0, 0, NULL);
  assert(fileMapping != INVALID_HANDLE_VALUE);

  LPVOID fileMapView = MapViewOfFile(fileMapping, FILE_MAP_READ, 0, 0, 0);
  auto fileMapViewChar = (const char*)fileMapView;
  assert(fileMapView != NULL);
#else

  FILE* f;
  long file_size;
  struct stat sb;
  char* p;
  int fd;

  (*len) = 0;

  f = fopen(filename, "r");
  fseek(f, 0, SEEK_END);
  file_size = ftell(f);
  fclose(f);

  fd = open(filename, O_RDONLY);
  if (fd == -1) {
    perror("open");
    return NULL;
  }

  if (fstat(fd, &sb) == -1) {
    perror("fstat");
    return NULL;
  }

  if (!S_ISREG(sb.st_mode)) {
    fprintf(stderr, "%s is not a file\n", "lineitem.tbl");
    return NULL;
  }

  p = (char*)mmap(0, (size_t)file_size, PROT_READ, MAP_SHARED, fd, 0);

  if (p == MAP_FAILED) {
    perror("mmap");
    return NULL;
  }

  if (close(fd) == -1) {
    perror("close");
    return NULL;
  }

  (*len) = (size_t)file_size;

  return p;

#endif
}

static const char* get_file_data(size_t* len, const char* filename) {
  size_t data_len = 0;
  const char* data = NULL;
  data = mmap_file(&data_len, filename);
  (*len) = data_len;
  return data;
}



obj_t LoadObjAndConvert(const char* filename) {
    
  obj_t o;
  o.vb            = 0;
  o.num_triangles = 0;
  o.num_points    = 0;
  tinyobj_attrib_t attrib;
  tinyobj_shape_t* shapes = NULL;
  size_t num_shapes;
  tinyobj_material_t* materials = NULL;
  size_t num_materials;

  size_t data_len = 0;
  const char* data = get_file_data(&data_len, filename);
  if (data == NULL) {
    exit(-1);
    /* return 0; */
  }
  printf("filesize: %d\n", (int)data_len);

  {
    unsigned int flags = TINYOBJ_FLAG_TRIANGULATE;
    int ret = tinyobj_parse_obj(&attrib, &shapes, &num_shapes, &materials,
                                &num_materials, data, data_len, flags);
    if (ret != TINYOBJ_SUCCESS) {
    return o;
    }

    printf("# of shapes    = %d\n", (int)num_shapes);
    printf("# of materiasl = %d\n", (int)num_materials);

    /*
    {
      int i;
      for (i = 0; i < num_shapes; i++) {
        printf("shape[%d] name = %s\n", i, shapes[i].name);
      }
    }
    */
  }
  {
    
    float* vb;
    /* std::vector<float> vb; //  */
    size_t face_offset = 0;
    size_t i;

    /* Assume triangulated face. */
    size_t num_triangles = attrib.num_face_num_verts;
    size_t stride = 9; /* 9 = pos(3float), normal(3float), color(3float) */

    vb = (float*)malloc(sizeof(float) * stride * num_triangles * 3);

    for (i = 0; i < attrib.num_face_num_verts; i++) {
      size_t f;
      assert(attrib.face_num_verts[i] % 3 ==
             0); /* assume all triangle faces. */
      for (f = 0; f < (size_t)attrib.face_num_verts[i] / 3; f++) {
        size_t k;
        float v[3][3];
        float n[3][3];
        float c[3];
        float len2;

        tinyobj_vertex_index_t idx0 = attrib.faces[face_offset + 3 * f + 0];
        tinyobj_vertex_index_t idx1 = attrib.faces[face_offset + 3 * f + 1];
        tinyobj_vertex_index_t idx2 = attrib.faces[face_offset + 3 * f + 2];

        for (k = 0; k < 3; k++) {
          int f0 = idx0.v_idx;
          int f1 = idx1.v_idx;
          int f2 = idx2.v_idx;
          assert(f0 >= 0);
          assert(f1 >= 0);
          assert(f2 >= 0);

          v[0][k] = attrib.vertices[3 * (size_t)f0 + k];
          v[1][k] = attrib.vertices[3 * (size_t)f1 + k];
          v[2][k] = attrib.vertices[3 * (size_t)f2 + k];
        }

        if (attrib.num_normals > 0) {
          int f0 = idx0.vn_idx;
          int f1 = idx1.vn_idx;
          int f2 = idx2.vn_idx;
          if (f0 >= 0 && f1 >= 0 && f2 >= 0) {
            assert(f0 < (int)attrib.num_normals);
            assert(f1 < (int)attrib.num_normals);
            assert(f2 < (int)attrib.num_normals);
            for (k = 0; k < 3; k++) {
              n[0][k] = attrib.normals[3 * (size_t)f0 + k];
              n[1][k] = attrib.normals[3 * (size_t)f1 + k];
              n[2][k] = attrib.normals[3 * (size_t)f2 + k];
            }
          } else { /* normal index is not defined for this face */
            /* compute geometric normal */
            CalcNormal(n[0], v[0], v[1], v[2]);
            n[1][0] = n[0][0];
            n[1][1] = n[0][1];
            n[1][2] = n[0][2];
            n[2][0] = n[0][0];
            n[2][1] = n[0][1];
            n[2][2] = n[0][2];
          }
        } else {
          /* compute geometric normal */
          CalcNormal(n[0], v[0], v[1], v[2]);
          n[1][0] = n[0][0];
          n[1][1] = n[0][1];
          n[1][2] = n[0][2];
          n[2][0] = n[0][0];
          n[2][1] = n[0][1];
          n[2][2] = n[0][2];
        }

        for (k = 0; k < 3; k++) {
          vb[(3 * i + k) * stride + 0] = v[k][0];
          vb[(3 * i + k) * stride + 1] = v[k][1];
          vb[(3 * i + k) * stride + 2] = v[k][2];
          vb[(3 * i + k) * stride + 3] = n[k][0];
          vb[(3 * i + k) * stride + 4] = n[k][1];
          vb[(3 * i + k) * stride + 5] = n[k][2];

          /* Use normal as color. */
          c[0] = n[k][0];
          c[1] = n[k][1];
          c[2] = n[k][2];
          len2 = c[0] * c[0] + c[1] * c[1] + c[2] * c[2];
          if (len2 > 0.0f) {
            float len = (float)sqrt((double)len2);

            c[0] /= len;
            c[1] /= len;
            c[2] /= len;
          }

          vb[(3 * i + k) * stride + 6] = (c[0] * 0.5f + 0.5f);
          vb[(3 * i + k) * stride + 7] = (c[1] * 0.5f + 0.5f);
          vb[(3 * i + k) * stride + 8] = (c[2] * 0.5f + 0.5f);
        }
      }
      face_offset += (size_t)attrib.face_num_verts[i];
    }
    if (num_triangles > 0) {
      glGenBuffers(1, &o.vb);
      glBindBuffer(GL_ARRAY_BUFFER, o.vb);
      glBufferData(GL_ARRAY_BUFFER, num_triangles * 3 * stride * sizeof(float),
                   vb, GL_STATIC_DRAW);
      o.num_triangles = (int)num_triangles;
      o.num_points = (int)num_triangles * 3;
    }

    free(vb);

    
  }

  tinyobj_attrib_free(&attrib);
  tinyobj_shapes_free(shapes, num_shapes);
  tinyobj_materials_free(materials, num_materials);
  return o;
}
