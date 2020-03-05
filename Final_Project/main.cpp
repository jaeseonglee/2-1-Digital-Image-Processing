
/*	영상처리 프로그래밍 기말 프로젝트
	작성자 : 컴퓨터 공학과 20165153 이재성
	프로그램 설명 : 
			제목 : 그림 맞추기 게임
				- 키보드로부터 입력받아 영상의 출력 및 직선그리기 함수를 통해 영상을 분할된 영상을 출력
				- 분할된 영상중 일부를 무작위로 골라 무작위 영상처리를 적용하고 키보드로부터 어디의 영상인지 문제를 낸다.
*/

#include <opencv2/opencv.hpp>	//opencv 함수들을 사용하기위한 전처리문
#include <stdlib.h>			
#include <time.h>			
#include <stdio.h>			

using namespace cv;		
using namespace std;

void devideImage(Mat img, int num);		// 영상을 분할하는 devideImage함수
void rectImage(Mat img, int num);		// 분할된 영상에서 무작위 영상처리 효과를 적용하고 문제를 내는 rectImage함수

int main(int argc, char* argv[]) {			// main함수			
	Mat image, otherImage;		// 영상 정보를 저장할 image, otherImage 변수 선언
	srand(time(NULL));						
	int num = 0;							// 키보드로부터 입력받아 값을 저장할 정수형 num 선언 및 0으로 초기화

	printf("영상 선택\n1.구름\t\t2.비둘기\t3.분수대\t4.선착장\n번호 입력 : ");	// 키보드로부터 어떤 영상을 출력할지 출력
	scanf_s("%d", &num);			// 입력받은 정수를 num에 저장

	switch (num) {						// num에 값에 따라 finalProject에 있는 영상 출력
	case 1:
		image = imread("구름.jpg");			// imread로 영상을 image에 저장
		break;
	case 2:
		image = imread("비둘기.jpg");
		break;
	case 3:
		image = imread("분수대.jpg");
		break;
	case 4:
		image = imread("선착장.jpg");
		break;
	default :
		break;
	}
	if (!image.data) {			// 영상을 읽지 못하면 -1을 반환하고 프로그램 종료
		printf("영상을 읽지 못해 에러가 발생했습니다...\n");
		return -1;
	}
	imshow("Basie image", image);	// swtich문을 통해 저장된 image 영상을 출력
	waitKey(1);						// 1초동안 키를 입력받기를 기다리지만, 사실 적용되는건 영상출력이후 다음 코드를 실행한다. 
	
	otherImage = image.clone();		// clone함수를 통해 otherImage변수에 복사한다. 메모리를 따로 가진다는 장점이 있다.

	printf("영상선택분할\n1.4분할(2X2)\n2.9분할(3X3)\n3.16분할(4X4)\n번호 입력 : ");		// 영상을 몇분할로 나눌지 출력
	scanf_s("%d", &num);				// 다시 입력받은 정수를 num에 새로 저장
	putText(otherImage, "Devide Image", cvPoint(0, 50), FONT_HERSHEY_SIMPLEX, 1, cvScalar(0, 0, 0), 2);
									// num에 값에 따라 분할될 이미지에 putText함수를 통해 Devide Image라고 글자를 출력한다.
	destroyAllWindows();			// Basic image창을 닫아준다.

	if (num > 0 && num < 4)				// 지정된 값 이외의 값을 입력하면 프로그램 종료
		devideImage(otherImage, num + 1);
	else {
		printf("1,2,3번 외의 번호를 입력해 실행을 취소합니다.");
		return -1;
	}
	
	imshow("divide image", otherImage);	// Devide Image라고 글씨를 쓰고 직선을 통해 이미지를 분할시킨 영상을 출력
	waitKey(1);							

	rectImage(image, num + 1);	// 분할된 이미지를 통해 문제를 내는 rectImage 함수
										
	system("pause");			// rectImage 함수를 통해 문제 출력이 끝나면 창을 멈추고
	destroyAllWindows();		// 어떤 값이든 입력되면 destoryAllWindow()함수를 통해 모든 영상을 닫는다.
	return 0;					// 0을 반환
}

void devideImage(Mat img, int num) {		// 영상이 저장된 img와 사용자가 입력한 정수 num을 매개변수로 갖는다.
	int i, j;								// 반복문에서 사용할 변수
	for (i = 1; i < num; i++) {			// 영상을 분할할 직선을 그리기 위해 얼만큼 분할할지에 대한 반복문
		for (j = 1; j < num; j++) {
			line(img, cvPoint(img.cols / num * i, 0), cvPoint(img.cols / num * i, img.rows), CvScalar(100, 200, 100), 2);
			line(img, cvPoint(0, img.rows / num * j), cvPoint(img.cols, img.rows / num * j), CvScalar(100, 200, 100), 2);
		
			/*	line(이미지,X,Y,색상,직선의 크기)
				line함수를 통해 X에서 부터 Y까지 어떤 색상으로 어떤크기만큼 직선을 그린다.
				cvPoint함수를 통해 위치를 지정해주고, 영상의 가로축과 세로축을 기준으로 직선을 그린다.
				
				CvScalar(Bule,Grenn,Red) BGR의 순서인것에 유의한다.*/
		}
	}
}

void rectImage(Mat img, int num) {				// 이미지의 일부분을 잘라 무작위 영상처리를 적용하는 rectImage함수
	Mat partImg;					// 분할될 영상을 따로 저장할 partImg 선언
	int ranCol = rand() % num;		// 0 ~ num-1의 값을 갖는다.
	int	ranRow = rand() % num;		// 무작위 행과 열이 정해지면 영상의 무작위 부분을 선택한다.
	int ranChange = rand() % 11;	// 0~10의 값을 갖는다. 이 변수를 통해 무작위 영상처리를 선택한다.
	int x, y;						// 키보드로부터 값을 저장할 변수 x,y선언

	Rect rect(img.cols / num * ranCol, img.rows / num * ranRow, img.cols / num, img.rows / num);		
	/*	관심영역을 자르는 rect(X,Y,W,H)
		X의 위치에서 W만큼, Y의 위치 H만큼 자른다.
		여기서는 분할된 영상에 따라 N등분으로 나누어진 영상에서 그 다음 N등분으로 나누어진 영상까지 이미지를 잘라준다.
	*/
	
	img = img(rect);				// 그렇게 잘린 이미지를img에 저장한다.
	partImg = img.clone();			// 분할된 영상을 따로 저장한다, 정답 영상에 출력에 사용한다.

	printf("일부 영상이 바뀌었습니다!!!\n");		// 문제 출제
	switch (ranChange){					// ranChange 값에 따라 분할된 영상에 각각의 영상처리를 적용한다.
										// 어떤 영상처리가 적용되었는지도 출력해주고 swtich문을 빠져나간다.
	case 0 :
		img += CvScalar(100, 100, 100);						// 영상에 100만큼 더함으로써 이미지를 밝게 한다.
		printf("힌트: 영상의 밝기 값이 증가했습니다!\n");
		break;
	case 1:
		img -= CvScalar(100, 100, 100);						// 영상에 100만큼 빼줌으로써 이미지를 어둡게 한다.
		printf("힌트: 영상의 밝기 값이 감소했습니다!\n");
		break;
	case 2:
		img = CvScalar(255, 255, 255) - img;		// CvScalar를 통해 만들어진 흰색에서 영상의 값만큼 빼주어 이미지의 역상을 저장한다.
		printf("힌트: 영상이 역상계산을 통해 바뀌었습니다!\n");
		break;
	case 3:
		img -= CvScalar(0, 255, 255);			// 이미지에서 파란색을 제외하고 다른 값들을 0으로 만들어 이미지의 파란색을 분리한다.
		printf("힌트: 영상의 BLUE값만 분리되었습니다!\n");
		break;
	case 4:
		img -= CvScalar(255, 0, 255);			// 이미지에서 초록색을 제외하고 다른 값들을 0으로 만들어 이미지의 초록색을 분리한다.
		printf("힌트: 영상의 GREEN값만 분리되었습니다\n");
		break;
	case 5:
		img -= CvScalar(255, 255,0 );			// 이미지에서 빨간색을 제외하고 다른 값들을 0으로 만들어 이미지의 빨간색을 분리한다.
		printf("힌트: 영상의 RED값만 분리되었습니다n");
		break;
	case 6:
		flip(partImg, img, 1);						// filp함수를 통해 영상을 반전시킨다. 1의 값으로 좌우반전이 적용된다.
		printf("힌트: 영상이 좌우로 반전되었습니다!\n");
		break;
	case 7:
		flip(partImg, img, 0);						// 영상반전, 0의 값으로 상하반전이 적용된다.
		printf("힌트: 영상이 상하로 반전되었습니다!\n");
		break;
	case 8:
		flip(partImg, img, -1);						// 영상반전, -1의 값으로 상하좌우반전이 적용된다.
		printf("힌트: 영상이 상하좌우로 반전되었습니다!\n");
		break;
	case 9:
		GaussianBlur(partImg, img, Size(0, 0), 3);	// 가우시안 필터, 블러링을 통해 영상이 흐려져 보이게 적용해준다.
		printf("힌트: 영상에 가우시안 필터가 적용되었습니다!\n");
		break;
	case 10:
		Laplacian(partImg, img, 0);		// Laplacian을 통해 영상에 라플라시안을 적용해준다.
		printf("힌트: 영상에 라플라시안 필터가 적용되었습니다!\n");
		break;
	default:	
		break;
	}

	imshow("change!!", img);			// swtich문을 통해 바뀐 img를 출력한다.
	waitKey(1);

	printf("이 영상은 전체 영상의 원래 몇번째 행의 몇번째 열일까요? :\n");		// 본래 영상의 행과 열을 맞추는 문제
	scanf_s("%d %d", &x, &y);		// 입력받은 두 정수를 x,y에 저장

	if ((y == ranCol + 1) && (x == ranRow + 1))			// 이미지의 행과 열이 반대로 되어있음에 유의한다.
		printf("정답입니다!\n");
	else
		printf("오답입니다..\n%d행의 %d열이었습니다..\n",ranRow+1,ranCol+1);

	printf("일부 영상이 복구됩니다.\n");		
	imshow("partImage", partImg);			// 처음 분리된 영상을 저장했던 partImg출력을 통해 정답화면 출력
	waitKey(1);
}