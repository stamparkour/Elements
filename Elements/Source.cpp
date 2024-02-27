#include <vector>
#include <fstream>
struct Element {
	const char* name;
	bool discovered;
};

struct Recipe {
	int a;
	int b;
	int out;
};

std::vector<Element> elements;
std::vector<Recipe> recipes;

int getElementIndex(const char* name) {
	int v = 0;
	if (sscanf_s(name, "%i", &v) == 1) {
		return v;
	}
	for (int i = 0; i < elements.size(); i++) {
		if (!strcmp(name, elements[i].name)) {
			return i;
		}
	}
	return -1;
}

int getRecipe(int a, int b) {
	for (int i = 0; i < recipes.size(); i++) {
		if (recipes[i].a == a && recipes[i].b == b) {
			return i;
		}
	}
	for (int i = 0; i < recipes.size(); i++) {
		if (recipes[i].a == b && recipes[i].b == a) {
			return i;
		}
	}
	return -1;
}

void ParseFile(std::fstream& file) {
	char buffer1[256];
	char buffer2[16];
	char buffer3[16];
	char buffer4[16];
	while (file) {
		file.getline(buffer1, 256);

		if (buffer1[0] == 0) continue;
		else if (sscanf_s(buffer1, "e %s %c", buffer2, 16, buffer3, 16) == 2) {
			char* v = new char[16];
			strcpy_s(v, 16, buffer2);
			if (getElementIndex(v) != -1) throw 4;
			elements.push_back({ v, buffer3[0] == 'd'});
		}
		else if (sscanf_s(buffer1, "e %s", buffer2, 16) == 1) {
			char* v = new char[16];
			strcpy_s(v, 16, buffer2);
			if (getElementIndex(v) != -1) throw 4;
			elements.push_back({ v, false });
		}
		else if (sscanf_s(buffer1, "r %s %s %s", buffer2, 16, buffer3, 16, buffer4, 16) == 3) {
			int a = getElementIndex(buffer2);
			int b = getElementIndex(buffer3);
			int o = getElementIndex(buffer4);
			if (a == -1 || b == -1 || o == -1) 
				throw 2;
			if (getRecipe(a, b) != -1) 
				throw 3;
			recipes.push_back({ a, b, o });
		}
		else {
			throw 1;
		}
	}
}

void printElements() {
	for (int i = 0; i < elements.size(); i++) {
		if(elements[i].discovered) printf("%s\r\n", elements[i].name);
	}
	printf("\r\n");
}

void readLine() {
	char buffer1[16];
	char buffer2[16];
	while (scanf_s("%s %s", buffer1, 16, buffer2, 16) != 2);
	
	int a = getElementIndex(buffer1);
	int b = getElementIndex(buffer2);
	if (a == -1 || b == -1) {
		printf("\r\n");
		return;
	}
	if (!elements[a].discovered || !elements[b].discovered) {
		printf("\r\n");
		return;
	}
	int i = getRecipe(a, b);
	if (i == -1) {
		printf("\r\n");
		return;
	}
	elements[recipes[i].out].discovered = true;
	printf("unlocked %s!\r\n\r\n", elements[recipes[i].out].name);
}

int main(int argc, char* argv[]) {
	std::fstream file{ argv[1], std::ios::in };
	ParseFile(file);
	while (true)
	{
		printElements();
		readLine();
	}
}