extern FILE* yyin;
extern int yylineno;
extern yytoken_kind_t lastToken;

const char * getAttribute();
char const* tokenToString(yytoken_kind_t const tok);


void myUnput(const char * text);
void myUnput2(const char * text, int si, int ei);


#define true 1
#define false 0
#define bool int
bool handleImplicitSemicolon(const char * condition, const char * input);


#define MY_MAX_BUFFER_SIZE 5000
#define MIN_FREE_SPACE 10
int mystrcat(char ** dest, const char * src, const unsigned int DEST_SIZE);


const char * filterText(const char * text, int mylen);

void handleWarningUnknown2(const char * text, int mylen);
void handleWarningUnknown(const char * text);
void handleWarningSkip(const char * text);
void handleWarningSkip2(const char * text, int mylen);