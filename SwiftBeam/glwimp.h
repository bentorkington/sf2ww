/* glwimp.h */

#define WIN_BORDER_WIDTH 1
#define WIN_BORDER_GAP 2
#define WIN_TITLE_HEIGHT	18

// 18


struct rect {
	GLfloat	left;
	GLfloat	top;
	GLfloat	width;
	GLfloat height;
};
typedef struct rect LBRect;

struct windowprops {
	GLfloat	border_width;
	GLfloat border_gap;

};
typedef struct windowprops LBWinProps;

struct view {
	struct view		*parent;
	struct view		*nextView;
	LBRect	rect;
	struct view		*subViews;
	int  (*	delegate)(struct view *, int, int, int, int, int, int);
	void (*	renderFunc)(struct view *);
	LBWinProps		properties;
};
typedef struct view LBView;

struct window {
	struct view *view;
	LBView *iconView;
	LBView *titleView;
	LBView *areaView;
	int	windowFlags;
	const char *title;
};
typedef struct window LBWindow;


LBView *addView(LBView *view);
LBView *addSubView(LBView *view, void *renderFunc, void *delegate, int left, int top, int width, int height);
LBWindow *createWindow(int left, int top, int width, int height);
void DrawPlainView(GLint left, GLint top, GLint width, GLint height);
void DrawMyFrame(LBView *view);
void DrawTileObj(u16 tileid, u16 palette, int left, int top, int width, int height);
void DrawTileScr1(u16 tileid, u16 palette, int left, int top, int width, int height);
void DrawFrameAndTitle(LBView *view, GLint screenHeight);
void DrawTitleBar(LBView *view);
void DrawWindowButton(LBView *view);
int DragBarClicked(LBView *view, int button, int status, int x, int y, int scrx, int scry);
int WindowButtonClicked(LBView *view);
int checkMouse(LBView *view, int button, int state, int x, int y, int scrx, int scry);
void DrawNull(LBView *view);
int ClickNull(LBView *view);
void glwimp_root_resize (int screenWidth, int screenHeight);
void glwimp_init(int screenWidth, int screenHeight);
