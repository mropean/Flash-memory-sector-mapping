#include "섹터 매핑.h"
const char* sector_data_f = "Sector_data_File.txt";
const char* sector_mapping_f = "Sector_mapping_File.txt";
const char* sector_spareblock = "Sector_spareBlock.txt";

void
input_Command() {
	char command[20];	// 명령어가 저장될 변수
	int count = 0;		// 토큰 위치
	char* tok;			// 토큰 포인터

	char s_com[10] = "";	// 분리한 토큰의 문자열을 저장할 배열
	int i_com = 0;			// 분리한 토큰의 수를 저장할 변수
	char c_com[2] = "";		// 분리한 토큰의 데이터를 저장할 배열

	cout << "실행하실 명령어를 입력해주세요 >> "; cin.getline(command, '\n');

	tok = strtok(command, " ");		// 문자열을 공백을 기준으로 토큰을 만든다.
	while (tok) { //토큰이 없을 때까지 반복
		count++;
		switch (count) {
		case 1:  //명령어 부
			strcpy(s_com, tok);		// char배열 s_com 에 저장
			break;
		case 2: //첫 번째 매개변수 
			i_com = atoi(tok);		// int형 변수 i_com 에 저장, 보통 문자열에 저장된 숫자는 아스키코드로 되어있어
			break;					// atoi를 사용하여 아스키코드로 저장된 숫자를 정수로 바꿔준다.
		case 3: //두 번째 매개변수
			strcpy(c_com, tok);		// char형 변수 c_com 에 저장
			break;
		}
		tok = strtok(NULL, " \n"); //다음 토큰으로 넘어감
	}

	switch (s_com[0]) {
	case 'i': {			// init 함수 실행
		init(i_com);
		break;
	}
	case 'w':			// write 함수 실행
		FTL_write(i_com, c_com);
		break;
	case 'r':			// read 함수 실행
		FTL_read(i_com);
		break;
	case 'e':			// erase 함수 실행
		Flash_erase(i_com);
		break;
	case 'o':			// 프로그램 종료
		exit(1);
	default: {
		cout << "잘못 입력하셨습니다." << endl;
		cin.clear();	// 입력버퍼 비우기
		cin.ignore(INT_MAX, '\n');	// 잘못된 입력일 경우 버퍼를 비워 무한반복을 예방한다.
	}
	}

}

void
init(int mb_Size) {
	ofstream write_f(sector_data_f);		// c++ 형식으로 파일 생성 및 열기
	ofstream write_mf(sector_mapping_f);	// 매핑 테이블용 파일 생성
	ofstream write_sf(sector_spareblock);	// 스페어 블록용 파일 생성

	// 스페어 블록을 이용하기 위해 생성한다.
	// 기존에 생성하려는 블록갯수 - 1 이 데이터 파일에 만들어지고
	// 남은 1 블록은 스페어 블록으로 사용한다.
	for (int var = 0; var < mb_Size; var++) {	// 입력된 크기만큼 메가바이트 생성
		for (int Block = 0; Block < 64; Block++) {	// 1메가바이트당 64개의 블록 생성
			if (Block == 63 && var == mb_Size - 1) {	// 스페어 블록용 데이터 파일 초기화 하기, 스페어 블록은 1블록크기
				for (int Sector = 0; Sector < 32; Sector++) {
					for (int i = 0; i < 510; i++) {					// 한 줄당 512개의 공백 + 줄넘김으로 512B 를 만든다.
						write_sf << " ";
					}
					write_sf << endl;
				}
				break;
			}
			for (int Sector = 0; Sector < 32; Sector++) {	// 1블록당 32개의 섹터 생성
				for (int i = 0; i < 510; i++) {					// 한 줄당 510개의 공백 줄넘김으로 512B 를 만든다.
					write_f << " ";
				}
				
				if (Block == 62 && Sector == 31) {			// 파일의 마지막줄은 줄넘김이 없게 추가로 2B의 데이터를 집어넣는다.
					write_f << "  ";
					break;
				}
				write_f << endl;
			}
		}
	}
	write_sf.close();			// 스페어 파일 닫기
	write_f.close();			// 데이터 파일 닫기

	// 매핑테이블 용 파일
	for (int var = 0, num = 0; var < mb_Size; var++) {
		for (int Block = 0; Block < 64; Block++) {
			if (var == mb_Size-1 && Block == 63) {		// 스페어 블록의 섹터 번호는 지정하지 않는다.
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

	cout << "정적 테이블로 만들었기 때문에 LSN = PSN 입니다." << endl;
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
	FILE* dFile = fopen("Sector_data_File.txt", "r+");		// 데이터파일을 r+ 모드로 열어서 파일을 이어서 작성할 수 있게 한다.
	FILE* sFile = fopen("Sector_spareBlock.txt", "r+");		// 스페어 파일을 r+ 모드로 열어서 파일을 이어서 작성할 수 있게 한다.
	char f_data[2] = "";
	int quotient, remainder, repeat = 1;								// 몫과 나머지를 저장할 변수, 반복을 위한 변수
	int flash_write = 0, flash_erase = 0;

	fseek(dFile, sector_num * 512, SEEK_SET);	// 입력된 섹터의 번호 * 512로 파일의 원하는 줄로 이동
	fgets(f_data, 2, dFile);						// 파일의 데이터를 읽어오는데
	//판단 1. 데이터가 있는지 판단
	if (f_data[0] != ' ') {
		// 판단 2. 현재 작성한 섹터의 블록을 찾아내고 블록안에서 작성할 섹터를 제외한 다른 데이터들 스페어로 옮기기
		quotient = sector_num / 32; remainder = sector_num % 32;		// 입력된 섹터에 32를 나누어 몫은 블록의 번호, 나머지는 블록안의 섹터의 순서로 지정
		fseek(dFile, (quotient * 32) * 512, SEEK_SET);		// 입력하는 섹터의 첫번째 섹터로 이동
		fseek(sFile, 0, SEEK_SET);							// 스페어 섹터의 처음으로 이동


		// 실제로는 flash write 함수가 실행되어야 하지만 시간관계상 스페어블록에 작성된 데이터를
		// 데이터 파일로 복사한다. 입력한 데이터만 정상적으로 flash_write 함수를 실행시키게 했다.	
		// 데이터 파일 스페어 파일로 복사하기
		while (repeat != 33) {
			if ((sector_num * 512) == ftell(dFile)) {		// 현재 이 알고리즘이 실행되는 이유
				fseek(dFile, 512, SEEK_CUR);			// 데이터 파일의 다음줄로 이동
				fseek(sFile, 512, SEEK_CUR);			// 스페어 파일의 다음줄로 이동
				repeat++;
				continue;
			}
			fgets(f_data, 2, dFile);;						// 파일의 데이터를 읽어오는데
			if (f_data[0] == ' ') {
				fseek(dFile, 511, SEEK_CUR);			// 데이터 파일의 다음줄로 이동
				fseek(sFile, 512, SEEK_CUR);			// 스페어 파일의 다음줄로 이동
				repeat++;
			}
			else if (f_data[0] != ' ') {			// 데이터를 스페어에 똑같이 작성
				fputs(f_data, sFile);
				fseek(dFile, 511, SEEK_CUR);			// 데이터 파일의 다음줄로 이동
				fseek(sFile, 511, SEEK_CUR);			// 스페어 파일의 다음줄로 이동2
				flash_write++;
				repeat++;
			}
		}

		Flash_erase(quotient);		// 블록단위 지우기
		flash_erase++;
		cout << "재작성: " << flash_write << "회, 쓰기: 1번 지우기 연산: " << quotient << "블록 1회" << "스페어 블록 1회" << endl;
		// 지웠으니 다시 작성
		repeat = 1;
		fseek(dFile, (quotient * 32) * 512, SEEK_SET);		// 입력하는 섹터의 첫번째 섹터로 이동
		fseek(sFile, 0, SEEK_SET);							// 스페어 섹터의 처음으로 이동
		while (repeat != 33) {
			if ((remainder * 512) == ftell(sFile)) {		// 현재 이 알고리즘이 실행되는 이유
				fseek(dFile, 512, SEEK_CUR);			// 데이터 파일의 다음줄로 이동
				fseek(sFile, 512, SEEK_CUR);			// 스페어 파일의 다음줄로 이동
				repeat++;
				continue;
			}
			fgets(f_data, 2, sFile);;						// 파일의 데이터를 읽어오는데
			if (f_data[0] == ' ') {
				fseek(dFile, 511, SEEK_CUR);			// 데이터 파일의 다음줄로 이동
				fseek(sFile, 511, SEEK_CUR);			// 스페어 파일의 다음줄로 이동
				repeat++;
			}
			else if (f_data[0] != ' ') {			// 데이터를 스페어에 똑같이 작성
				fputs(f_data, dFile);
				fseek(dFile, 511, SEEK_CUR);			// 데이터 파일의 다음줄로 이동
				fseek(sFile, 511, SEEK_CUR);			// 스페어 파일의 다음줄로 이동
				repeat++;
			}
		}
		Spare_erased();				// 스페어블록도 지운다
	}
	// 없다면 작성
	Flash_write(sector_num, data);
	
	fclose(dFile);
	fclose(sFile);		// 열어놓은 두 파일 닫기
}

void
Flash_read(int sector_num) {
	char data[2] = "";
	FILE* dFile = fopen("Sector_data_File.txt", "r");	// r 모드로 파일을 열어서 오직 읽기만 가능하게 한다.
	fseek(dFile, sector_num * 512, SEEK_SET);	// 입력된 섹터의 번호 * 512로 파일의 원하는 줄로 이동
	fgets(data, 2, dFile);						// 파일의 데이터를 읽어오는데
	if (data[0] == ' ') {						// 공백이라면 불러올 데이터가 없으니 함수를 종료한다.
		cout << "불러올 데이터가 없습니다!" << endl;
		fclose(dFile);								// 파일을 닫는다.
		return;
	}
	cout << data[0];					// 화면상에 불러온 데이터 출력
	fclose(dFile);								// 파일을 닫는다.
}

void
Flash_write(int sector_num, char* data) {
	char find_data[2] = "";
	FILE* dFile = fopen("Sector_data_File.txt", "r+");	// r+ 모드로 열어서 파일을 이어서 작성할 수 있게 한다.

	fseek(dFile, sector_num * 512, SEEK_SET);	// 입력된 섹터의 번호 * 512로 파일의 원하는 줄로 이동
	fgets(find_data, 2, dFile);				// 만약 파일에서 읽어온 데이터가
	if (find_data[0] != ' ') {				// 공백이 아니라면 덮어쓰기 에러
		cout << "overwrite error!!!" << endl;
		fclose(dFile);							// 파일을 닫는다.
		return;
	}
	fseek(dFile, -1, SEEK_CUR);				// 앞에서 읽으려고 바이트를 이동했으니 다시 뒤로 파일 포인터를 넘긴다.
	fputs(data, dFile);						// 현재 포인터에서 입력된 데이터 입력
	fclose(dFile);							// 파일을 닫는다.

	cout << "done" << endl;
	return;
}

void
Flash_erase(int block_num) {
	int b_start, b_end;							// 블록안의 시작 섹터와 끝 섹터의 변수 값
	FILE* dFile = fopen("Sector_data_File.txt", "r+");

	// 지우기 연산은 블록단위
	b_start = block_num * 32;	// 블록의 첫번째 섹터의 위치
	b_end = b_start + 31;		// 블록의 마지막 섹터의 위치 0~31

	fseek(dFile, b_start * 512, SEEK_SET);	// 지우려는 블록의 첫번째 섹터로 이동
	// 첫번째 섹터의 맨앞에 포인터가 있다
	for (int Sector = 0; Sector < 32; Sector++) {	// 1블록당 32개의 섹터 생성
		for (int i = 0; i < 510; i++) {					// 한 줄당 510개의 공백 + 줄넘김으로 512B를 만든다.
			fputs(" ", dFile);
		}
		fputs("\n", dFile);
	}
	fclose(dFile);			// 파일을 닫는다.
	cout << block_num << "-th block erase." << endl;
	return;
}

// 섹터가 몇번째 줄에 작성한지 보는 함수인데 미사용.
int
sector_mapping(int sector_num) {
	int num, line_count = 0;
	char buffer[128];
	FILE* dFile = fopen("Sector_mapping_File.txt", "r+");	// r+ 모드로 열어서 파일을 이어서 작성할 수 있게 한다.

	while (fgets(buffer, 128, dFile) != NULL) {
		line_count++;
		if (line_count == sector_num+1) {
			printf("%d번째 줄 %d번 섹터: %s\n", line_count, sector_num, buffer);
			break;
		}
	}
	fclose(dFile);

	return 0;
}

void
Spare_erased() {			// 스페어 블록 지우기
	FILE* dFile = fopen("Sector_spareBlock.txt", "r+");

	// 지우기 연산은 블록단위
	fseek(dFile, 0, SEEK_SET);	// 지우려는 블록의 첫번째 섹터로 이동
	// 첫번째 섹터의 맨앞에 포인터가 있다
	for (int Sector = 0; Sector < 32; Sector++) {	// 1블록당 32개의 섹터 생성
		for (int i = 0; i < 510; i++) {					// 한 줄당 510개의 공백 + 줄넘김으로 512B를 만든다.
			fputs(" ", dFile);
		}
		fputs("\n", dFile);
	}
	fclose(dFile);			// 파일을 닫는다.
	return;
}