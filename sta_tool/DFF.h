# ifndef _DFF_H_
# define _DFF_H_
typedef enum {DFX = 0, DFC = 1}DFF_type;
class D_flipflop
{		
	public:
	int DA_index;
	int DB_index;	
//	int D_index;
	int Q_index;
	int SA_index;
	int Q_next;
	DFF_type type;
};

# endif
