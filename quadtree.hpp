// quadtree.hpp	-thatcher 9/15/1999 Copyright 1999-2000 Thatcher Ulrich

// Data structures for quadtree terrain storage.

// This code may be freely modified and redistributed.  I make no
// warrantees about it; use at your own risk.  If you do incorporate
// this code into a project, I'd appreciate a mention in the credits.
//
// Thatcher Ulrich <tu@tulrich.com>



#ifndef QUADTREE_HPP
#define QUADTREE_HPP


typedef unsigned short uint16;
typedef unsigned int uint32;
typedef short int16;
typedef int int32;


struct HeightMapInfo {
	float*	Data;
	int	XOrigin, ZOrigin;
	int	XSize, ZSize;
	int	RowWidth;
	int	Scale;

	float	Sample(int x, int z) const;
};


struct quadsquare;


// A structure used during recursive traversal of the tree to hold
// relevant but transitory data.
struct quadcornerdata {
	const quadcornerdata*	Parent;
	quadsquare*	Square;
	int	ChildIndex;
	int	Level;
	int	xorg, zorg;
	uint16	Y[4];	// ne, nw, sw, se
};


const int	MAX_QSQUARE_HEAP_SIZE = (1 << 20) * 32;	// 32 megs.
const int	QSQUARE_HEAP_BLOCK_SIZE = 100000;	// ~100K per block.
const int	MAX_QSQUARE_HEAP_BLOCKS = MAX_QSQUARE_HEAP_SIZE / QSQUARE_HEAP_BLOCK_SIZE;


struct quadsquare {
	quadsquare*	Child[4];

	uint16	Y[5];	// center, e, n, w, s

	// Memory management stuff.
	static void*	HeapBlock[MAX_QSQUARE_HEAP_BLOCKS];
	static int	HeapBlockCount;
	static int	SquareCount;
	static void**	FreeList;
	
// public:
	quadsquare(quadcornerdata* pcd);
	~quadsquare();
	void*	operator new(size_t);
	void	operator delete(void* o, size_t s);

	static quadsquare*	NewQuadSquare(quadcornerdata* pcd);	// work around apparent VC bug.

	static void	OpenAllocator();
	static void	CloseAllocator();
	static void	DeleteAllSquares();

	void	Serialize(const quadcornerdata& cd, CArchive& ar);
	
	void	AddHeightMap(const quadcornerdata& cd, const HeightMapInfo& hm);
	void	Deresolve(const quadcornerdata& cd, const HeightMapInfo& hm, int TargetScale);
	
	void	StaticCullData(const quadcornerdata& cd, float ThresholdDetail);
//	void	ConsistencyCheck();
	void	DeCrack(const quadcornerdata& cd, int TargetLevel);
	int	CountNodes();
	
	float	GetHeight(const quadcornerdata& cd, float x, float z);
	
private:
	void	ResetTree();
	void	StaticCullAux(const quadcornerdata& cd, float ThresholdDetail, int TargetLevel);

	quadsquare*	GetNeighbor(int dir, const quadcornerdata& cd);
	void	CreateChild(int index, const quadcornerdata& cd);
	void	SetupCornerData(quadcornerdata* q, const quadcornerdata& pd, int ChildIndex);
};


#endif // QUADTREE_HPP
