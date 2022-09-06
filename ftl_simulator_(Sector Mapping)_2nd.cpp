#include "���� ����.h"
const char* sector_data_f = "Sector_data_File.txt";
const char* sector_mapping_f = "Sector_mapping_File.txt";
const char* sector_spareblock = "Sector_spareBlock.txt";

void
input_Command() {
	char command[20];	// ��ɾ ����� ����
	int count = 0;		// ��ū ��ġ
	char* tok;			// ��ū ������

	char s_com[10] = "";	// �и��� ��ū�� ���ڿ��� ������ �迭
	int i_com = 0;			// �и��� ��ū�� ���� ������ ����
	char c_com[2] = "";		// �и��� ��ū�� �����͸� ������ �迭

	cout << "�����Ͻ� ��ɾ �Է����ּ��� >> "; cin.getline(command, '\n');

	tok = strtok(command, " ");		// ���ڿ��� ������ �������� ��ū�� �����.
	while (tok) { //��ū�� ���� ������ �ݺ�
		count++;
		switch (count) {
		case 1:  //��ɾ� ��
			strcpy(s_com, tok);		// char�迭 s_com �� ����
			break;
		case 2: //ù ��° �Ű����� 
			i_com = atoi(tok);		// int�� ���� i_com �� ����, ���� ���ڿ��� ����� ���ڴ� �ƽ�Ű�ڵ�� �Ǿ��־�
			break;					// atoi�� ����Ͽ� �ƽ�Ű�ڵ�� ����� ���ڸ� ������ �ٲ��ش�.
		case 3: //�� ��° �Ű�����
			strcpy(c_com, tok);		// char�� ���� c_com �� ����
			break;
		}
		tok = strtok(NULL, " \n"); //���� ��ū���� �Ѿ
	}

	switch (s_com[0]) {
	case 'i': {			// init �Լ� ����
		init(i_com);
		break;
	}
	case 'w':			// write �Լ� ����
		FTL_write(i_com, c_com);
		break;
	case 'r':			// read �Լ� ����
		FTL_read(i_com);
		break;
	case 'e':			// erase �Լ� ����
		Flash_erase(i_com);
		break;
	case 'o':			// ���α׷� ����
		exit(1);
	default: {
		cout << "�߸� �Է��ϼ̽��ϴ�." << endl;
		cin.clear();	// �Է¹��� ����
		cin.ignore(INT_MAX, '\n');	// �߸��� �Է��� ��� ���۸� ��� ���ѹݺ��� �����Ѵ�.
	}
	}

}

void
init(int mb_Size) {
	ofstream write_f(sector_data_f);		// c++ �������� ���� ���� �� ����
	ofstream write_mf(sector_mapping_f);	// ���� ���̺�� ���� ����
	ofstream write_sf(sector_spareblock);	// ����� ��Ͽ� ���� ����

	// ����� ����� �̿��ϱ� ���� �����Ѵ�.
	// ������ �����Ϸ��� ��ϰ��� - 1 �� ������ ���Ͽ� ���������
	// ���� 1 ����� ����� ������� ����Ѵ�.
	for (int var = 0; var < mb_Size; var++) {	// �Էµ� ũ�⸸ŭ �ް�����Ʈ ����
		for (int Block = 0; Block < 64; Block++) {	// 1�ް�����Ʈ�� 64���� ��� ����
			if (Block == 63 && var == mb_Size - 1) {	// ����� ��Ͽ� ������ ���� �ʱ�ȭ �ϱ�, ����� ����� 1���ũ��
				for (int Sector = 0; Sector < 32; Sector++) {
					for (int i = 0; i < 510; i++) {					// �� �ٴ� 512���� ���� + �ٳѱ����� 512B �� �����.
						write_sf << " ";
					}
					write_sf << endl;
				}
				break;
			}
			for (int Sector = 0; Sector < 32; Sector++) {	// 1��ϴ� 32���� ���� ����
				for (int i = 0; i < 510; i++) {					// �� �ٴ� 510���� ���� �ٳѱ����� 512B �� �����.
					write_f << " ";
				}
				
				if (Block == 62 && Sector == 31) {			// ������ ���������� �ٳѱ��� ���� �߰��� 2B�� �����͸� ����ִ´�.
					write_f << "  ";
					break;
				}
				write_f << endl;
			}
		}
	}
	write_sf.close();			// ����� ���� �ݱ�
	write_f.close();			// ������ ���� �ݱ�

	// �������̺� �� ����
	for (int var = 0, num = 0; var < mb_Size; var++) {
		for (int Block = 0; Block < 64; Block++) {
			if (var == mb_Size-1 && Block == 63) {		// ����� ����� ���� ��ȣ�� �������� �ʴ´�.
				break;
			}
			for (int Sector = 0; Sector < 32; Sector++) {
				write_mf << num << endl;
				num++;
			}
		}
	}
	write_mf.close();

	cout << mb_Size << " megabytes flash memory" << endl;
	return;
}

void
Print_table() {
	ifstream File(sector_mapping_f);
	string read_data;

	cout << "���� ���̺�� ������� ������ LSN = PSN �Դϴ�." << endl;
	if (File.is_open()) {
		File.seekg(0, ios::end);
		int size = File.tellg();
		read_data.resize(size);
		File.seekg(0, ios::beg);
		File.read(&read_data[0], size);
		cout << read_data << endl;
	}
}

void
FTL_read(int sector_num) {
	Flash_read(sector_num);
	cout << ", " << "PSN: " << sector_num << endl;
	return;
}

void
	FTL_write(int sector_num, char* data) {
	FILE* dFile = fopen("Sector_data_File.txt", "r+");		// ������������ r+ ���� ��� ������ �̾ �ۼ��� �� �ְ� �Ѵ�.
	FILE* sFile = fopen("Sector_spareBlock.txt", "r+");		// ����� ������ r+ ���� ��� ������ �̾ �ۼ��� �� �ְ� �Ѵ�.
	char f_data[2] = "";
	int quotient, remainder, repeat = 1;								// ��� �������� ������ ����, �ݺ��� ���� ����
	int flash_write = 0, flash_erase = 0;

	fseek(dFile, sector_num * 512, SEEK_SET);	// �Էµ� ������ ��ȣ * 512�� ������ ���ϴ� �ٷ� �̵�
	fgets(f_data, 2, dFile);						// ������ �����͸� �о���µ�
	//�Ǵ� 1. �����Ͱ� �ִ��� �Ǵ�
	if (f_data[0] != ' ') {
		// �Ǵ� 2. ���� �ۼ��� ������ ����� ã�Ƴ��� ��Ͼȿ��� �ۼ��� ���͸� ������ �ٸ� �����͵� ������ �ű��
		quotient = sector_num / 32; remainder = sector_num % 32;		// �Էµ� ���Ϳ� 32�� ������ ���� ����� ��ȣ, �������� ��Ͼ��� ������ ������ ����
		fseek(dFile, (quotient * 32) * 512, SEEK_SET);		// �Է��ϴ� ������ ù��° ���ͷ� �̵�
		fseek(sFile, 0, SEEK_SET);							// ����� ������ ó������ �̵�


		// �����δ� flash write �Լ��� ����Ǿ�� ������ �ð������ ������Ͽ� �ۼ��� �����͸�
		// ������ ���Ϸ� �����Ѵ�. �Է��� �����͸� ���������� flash_write �Լ��� �����Ű�� �ߴ�.	
		// ������ ���� ����� ���Ϸ� �����ϱ�
		while (repeat != 33) {
			if ((sector_num * 512) == ftell(dFile)) {		// ���� �� �˰����� ����Ǵ� ����
				fseek(dFile, 512, SEEK_CUR);			// ������ ������ �����ٷ� �̵�
				fseek(sFile, 512, SEEK_CUR);			// ����� ������ �����ٷ� �̵�
				repeat++;
				continue;
			}
			fgets(f_data, 2, dFile);;						// ������ �����͸� �о���µ�
			if (f_data[0] == ' ') {
				fseek(dFile, 511, SEEK_CUR);			// ������ ������ �����ٷ� �̵�
				fseek(sFile, 512, SEEK_CUR);			// ����� ������ �����ٷ� �̵�
				repeat++;
			}
			else if (f_data[0] != ' ') {			// �����͸� ���� �Ȱ��� �ۼ�
				fputs(f_data, sFile);
				fseek(dFile, 511, SEEK_CUR);			// ������ ������ �����ٷ� �̵�
				fseek(sFile, 511, SEEK_CUR);			// ����� ������ �����ٷ� �̵�2
				flash_write++;
				repeat++;
			}
		}

		Flash_erase(quotient);		// ��ϴ��� �����
		flash_erase++;
		cout << "���ۼ�: " << flash_write << "ȸ, ����: 1�� ����� ����: " << quotient << "��� 1ȸ" << "����� ��� 1ȸ" << endl;
		// �������� �ٽ� �ۼ�
		repeat = 1;
		fseek(dFile, (quotient * 32) * 512, SEEK_SET);		// �Է��ϴ� ������ ù��° ���ͷ� �̵�
		fseek(sFile, 0, SEEK_SET);							// ����� ������ ó������ �̵�
		while (repeat != 33) {
			if ((remainder * 512) == ftell(sFile)) {		// ���� �� �˰����� ����Ǵ� ����
				fseek(dFile, 512, SEEK_CUR);			// ������ ������ �����ٷ� �̵�
				fseek(sFile, 512, SEEK_CUR);			// ����� ������ �����ٷ� �̵�
				repeat++;
				continue;
			}
			fgets(f_data, 2, sFile);;						// ������ �����͸� �о���µ�
			if (f_data[0] == ' ') {
				fseek(dFile, 511, SEEK_CUR);			// ������ ������ �����ٷ� �̵�
				fseek(sFile, 511, SEEK_CUR);			// ����� ������ �����ٷ� �̵�
				repeat++;
			}
			else if (f_data[0] != ' ') {			// �����͸� ���� �Ȱ��� �ۼ�
				fputs(f_data, dFile);
				fseek(dFile, 511, SEEK_CUR);			// ������ ������ �����ٷ� �̵�
				fseek(sFile, 511, SEEK_CUR);			// ����� ������ �����ٷ� �̵�
				repeat++;
			}
		}
		Spare_erased();				// ������ϵ� �����
	}
	// ���ٸ� �ۼ�
	Flash_write(sector_num, data);
	
	fclose(dFile);
	fclose(sFile);		// ������� �� ���� �ݱ�
}

void
Flash_read(int sector_num) {
	char data[2] = "";
	FILE* dFile = fopen("Sector_data_File.txt", "r");	// r ���� ������ ��� ���� �б⸸ �����ϰ� �Ѵ�.
	fseek(dFile, sector_num * 512, SEEK_SET);	// �Էµ� ������ ��ȣ * 512�� ������ ���ϴ� �ٷ� �̵�
	fgets(data, 2, dFile);						// ������ �����͸� �о���µ�
	if (data[0] == ' ') {						// �����̶�� �ҷ��� �����Ͱ� ������ �Լ��� �����Ѵ�.
		cout << "�ҷ��� �����Ͱ� �����ϴ�!" << endl;
		fclose(dFile);								// ������ �ݴ´�.
		return;
	}
	cout << data[0];					// ȭ��� �ҷ��� ������ ���
	fclose(dFile);								// ������ �ݴ´�.
}

void
Flash_write(int sector_num, char* data) {
	char find_data[2] = "";
	FILE* dFile = fopen("Sector_data_File.txt", "r+");	// r+ ���� ��� ������ �̾ �ۼ��� �� �ְ� �Ѵ�.

	fseek(dFile, sector_num * 512, SEEK_SET);	// �Էµ� ������ ��ȣ * 512�� ������ ���ϴ� �ٷ� �̵�
	fgets(find_data, 2, dFile);				// ���� ���Ͽ��� �о�� �����Ͱ�
	if (find_data[0] != ' ') {				// ������ �ƴ϶�� ����� ����
		cout << "overwrite error!!!" << endl;
		fclose(dFile);							// ������ �ݴ´�.
		return;
	}
	fseek(dFile, -1, SEEK_CUR);				// �տ��� �������� ����Ʈ�� �̵������� �ٽ� �ڷ� ���� �����͸� �ѱ��.
	fputs(data, dFile);						// ���� �����Ϳ��� �Էµ� ������ �Է�
	fclose(dFile);							// ������ �ݴ´�.

	cout << "done" << endl;
	return;
}

void
Flash_erase(int block_num) {
	int b_start, b_end;							// ��Ͼ��� ���� ���Ϳ� �� ������ ���� ��
	FILE* dFile = fopen("Sector_data_File.txt", "r+");

	// ����� ������ ��ϴ���
	b_start = block_num * 32;	// ����� ù��° ������ ��ġ
	b_end = b_start + 31;		// ����� ������ ������ ��ġ 0~31

	fseek(dFile, b_start * 512, SEEK_SET);	// ������� ����� ù��° ���ͷ� �̵�
	// ù��° ������ �Ǿտ� �����Ͱ� �ִ�
	for (int Sector = 0; Sector < 32; Sector++) {	// 1��ϴ� 32���� ���� ����
		for (int i = 0; i < 510; i++) {					// �� �ٴ� 510���� ���� + �ٳѱ����� 512B�� �����.
			fputs(" ", dFile);
		}
		fputs("\n", dFile);
	}
	fclose(dFile);			// ������ �ݴ´�.
	cout << block_num << "-th block erase." << endl;
	return;
}

// ���Ͱ� ���° �ٿ� �ۼ����� ���� �Լ��ε� �̻��.
int
sector_mapping(int sector_num) {
	int num, line_count = 0;
	char buffer[128];
	FILE* dFile = fopen("Sector_mapping_File.txt", "r+");	// r+ ���� ��� ������ �̾ �ۼ��� �� �ְ� �Ѵ�.

	while (fgets(buffer, 128, dFile) != NULL) {
		line_count++;
		if (line_count == sector_num+1) {
			printf("%d��° �� %d�� ����: %s\n", line_count, sector_num, buffer);
			break;
		}
	}
	fclose(dFile);

	return 0;
}

void
Spare_erased() {			// ����� ��� �����
	FILE* dFile = fopen("Sector_spareBlock.txt", "r+");

	// ����� ������ ��ϴ���
	fseek(dFile, 0, SEEK_SET);	// ������� ����� ù��° ���ͷ� �̵�
	// ù��° ������ �Ǿտ� �����Ͱ� �ִ�
	for (int Sector = 0; Sector < 32; Sector++) {	// 1��ϴ� 32���� ���� ����
		for (int i = 0; i < 510; i++) {					// �� �ٴ� 510���� ���� + �ٳѱ����� 512B�� �����.
			fputs(" ", dFile);
		}
		fputs("\n", dFile);
	}
	fclose(dFile);			// ������ �ݴ´�.
	return;
}