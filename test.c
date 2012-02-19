int (*kprintf)(char*, ...) = (int (*))0x100A17;
void (*exit)(int) = (void (*))0x10199B;

int main(){
	int i;
	for(i=0; i < 10; i++)	
		kprintf("#%d Hello, world!\n", i);
	exit(0);
}
