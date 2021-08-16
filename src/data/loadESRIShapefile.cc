#include <iostream>
#include <vector>

#include "util/Benchmark.hh"
#include "libshape/shapefil.h"

using namespace std;

void loadESRIShapefile(const char filename[]) {
  SHPHandle shapeHandle = SHPOpen(filename, "rb");
  int nEntities = shapeHandle->nRecords;
  int nShapeType = shapeHandle->nShapeType;

  uint64_t numPoints = 0;
  uint32_t numSegments = 0;
  vector<SHPObject*> shapes(nEntities);
  for (int i = 0; i < nEntities; i++) {
    SHPObject* shape = SHPReadObject(shapeHandle, i);
    if (shape == nullptr) {
      std::cerr << "Warning: error reading shape " << i << "\n";
      continue;
    }
    numPoints += shape->nVertices;
    numSegments += shape->nParts;
    shapes[i] = shape;
  }

  vector<uint32_t> startPoly(
      numSegments);  // preallocate correct number of polygons
  vector<float> points(numPoints * 2);  // preallocate correct number of floats
  uint32_t pointOffset = 0;             // track current point
  for (uint32_t i = 0; i < shapes.size(); i++) {
    int* start = shapes[i]->panPartStart;
    if (start == nullptr && shapes[i]->nParts != 0) {
      cerr << "Error null list of offsets shape=" << i
           << " for parts panpartstart\n";
      // TODO: // exception
      return;
    }

    for (uint32_t j = 0; j < shapes[i]->nParts; j++) {
      uint32_t numPointsPerSeg = j == shapes[i]->nParts - 1
                                     ? shapes[i]->nVertices - start[j]
                                     : start[j + 1] - start[j];
      for (uint32_t k = 0; k < numPointsPerSeg; k++) {
        points[pointOffset++] = shapes[i]->padfX[start[j] + k];
        points[pointOffset++] = shapes[i]->padfY[start[j] + k];
      }
    }
  }
  for (auto shape : shapes) SHPDestroyObject(shape);
  SHPClose(shapeHandle);
}

class PointInfo {
public:
	double meanLat, meanLon;
	uint32_t numPoints;
	uint32_t numSegments;
};

// compute the average of lat/lon and extract number of points and segments
void statsESRI(const char filename[], PointInfo* info) {
  SHPHandle shapeHandle = SHPOpen(filename, "rb");
  int nEntities = shapeHandle->nRecords;
  int nShapeType = shapeHandle->nShapeType;

  uint64_t numPoints = 0;
  uint32_t numSegments = 0;
	double sumLat = 0, sumLon = 0;
  for (int i = 0; i < nEntities; i++) {
    SHPObject* shape = SHPReadObject(shapeHandle, i);
    if (shape == nullptr) {
      std::cerr << "Warning: error reading shape " << i << "\n";
      continue;
    }
    numPoints += shape->nVertices;
    numSegments += shape->nParts;
		int* start = shape->panPartStart;
		for (uint32_t j = 0; j < shape->nParts-1; j++) {
      uint32_t numPointsPerSeg = start[j + 1] - start[j];
      for (uint32_t k = 0; k < numPointsPerSeg; k++) {
				sumLon += shape->padfX[start[j] + k];
        sumLat += shape->padfY[start[j] + k];
			}
		}
		// now for the last segment
		const uint32_t lastStart = start[shape->nParts-1];
		uint32_t numPointsPerSeg = shape->nVertices - lastStart;
		for (uint32_t k = 0; k < numPointsPerSeg; k++) {
			sumLon += shape->padfX[lastStart + k];
			sumLat += shape->padfY[lastStart + k];
		}
		SHPDestroyObject(shape);
	}
	info->meanLat = sumLat / numPoints;
	info->meanLon = sumLon / numPoints;
	info->numPoints = numPoints;
	info->numSegments = numSegments;
  SHPClose(shapeHandle);
}


void loadESRIDBF(const char filename[]) {
  DBFHandle dbf = DBFOpen(filename, "rb");
  int numFields = DBFGetFieldCount(dbf);
  char fieldName[12];  // must be at least 12 bytes
  int fieldWidth;
  int precision;
  for (int i = 0; i < numFields; i++) {
    DBFFieldType t =
        DBFGetFieldInfo(dbf, i, fieldName, &fieldWidth, &precision);
    cout << fieldName << '\t' << fieldWidth << '\t' << precision << '\n';
  }

  int recordCount = DBFGetRecordCount(dbf);
  int SQMI = DBFGetFieldIndex(dbf, "SQMI");  // get index of this field
  for (int i = 0; i < recordCount; i++) {
    cout << DBFReadStringAttribute(dbf, i, 0) << '\t'
				 << DBFReadStringAttribute(dbf, i, 1) << '\t'
				 << DBFReadStringAttribute(dbf, i, 2) << '\t'
         << DBFReadStringAttribute(dbf, i, 3) << '\t'
         << DBFReadStringAttribute(dbf, i, 4) << '\t'
         << DBFReadStringAttribute(dbf, i, 5) << '\t'
         << DBFReadStringAttribute(dbf, i, 7)
         << '\t'  // population
         //<< DBFReadStringAttribute(dbf, i, 7) << '\t' // lat?
         //<< DBFReadStringAttribute(dbf, i, 7) << '\t' // lon?
         << DBFReadStringAttribute(dbf, i, SQMI) << '\n';
  }
  DBFClose(dbf);
}
#if 0
template<typename... Args>
string buildString(Args...template) {
  if ()
}
#endif

void extractESRIDBF(const char filename[], PointInfo* info) {
  DBFHandle dbf = DBFOpen(filename, "rb");
  int numFields = DBFGetFieldCount(dbf);
  char fieldName[12];  // must be at least 12 bytes
  int fieldWidth;
  int precision;
  for (int i = 0; i < numFields; i++) {
    DBFFieldType t =
        DBFGetFieldInfo(dbf, i, fieldName, &fieldWidth, &precision);
    cout << fieldName << '\t' << fieldWidth << '\t' << precision << '\n';
  }

  int recordCount = DBFGetRecordCount(dbf);
  int SQMI = DBFGetFieldIndex(dbf, "SQMI");  // get index of this field
  for (int i = 0; i < recordCount; i++) {
		cout << DBFReadStringAttribute(dbf, i, 0) << '\t'
				 << DBFReadStringAttribute(dbf, i, 2) << '\t'
         << DBFReadStringAttribute(dbf, i, 3) << '\t'
         << DBFReadStringAttribute(dbf, i, 4) << '\t'
         << DBFReadStringAttribute(dbf, i, 5) << '\t'
         << DBFReadStringAttribute(dbf, i, 7)
         << '\t'  // population
         //<< DBFReadStringAttribute(dbf, i, 7) << '\t' // lat?
         //<< DBFReadStringAttribute(dbf, i, 7) << '\t' // lon?
         << DBFReadStringAttribute(dbf, i, SQMI) << '\n';
  }
  DBFClose(dbf);
}

int main() {
	PointInfo info;
	const string dir = getenv("GRAIL");
	const string counties_shp = dir + "/test/res/maps/USA_Counties.shp";
	//	Benchmark::benchmark(statsESRI, counties_shp, &info);
	const string counties_dbf = dir + "/test/res/maps/USA_Counties.dbf";
	//	Benchmark::benchmark(loadESRIDBF, counties_dbf, &info);
	loadESRIDBF(counties_dbf.c_str());
	
// //    string data = buildString(getenv("GRAILDATA"), "/maps/c_10nv20.dbf");
// //    loadESRIDBF((dir + "USA_Counties.dbf").c_str());
}
