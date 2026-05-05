class Object;

class Node {
private:
	int id;
	Object* objeto;
public:
Node(int id);
Object* getObjeto();
void setObjeto(Object* obj);
};
