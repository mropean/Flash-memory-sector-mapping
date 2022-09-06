#include "섹터 매핑.h"

void main() {
	cout << "******sector mapping simulation******" << endl;
	cout << "/ * 메뉴얼" << endl;
	cout << "  *ps: 데이터의 작성과 출력은 섹터 단위입니다." << endl;
	cout << "  *pps: 데이터를 지울 때는 블록 단위입니다." << endl;
	cout << "  *ppps: 명령어 실행 예제입니다." << endl;
	cout << "  * 메모리 생성: init (megaByte variable)" << endl;
	cout << "  * 데이터 작성: w (memory sector number) (data)" << endl;
	cout << "  * 데이터 출력: r (memory sector number)" << endl;
	cout << "  * 데이터 지움: e (memory block number) * /" << endl;
	cout << "  * 프로그램 종료: off * /" << endl;
	cout << "*****************************" << endl;
	while (1) {
		input_Command();
	}
}