
/*	����ó�� ���α׷��� �⸻ ������Ʈ
	�ۼ��� : ��ǻ�� ���а� 20165153 ���缺
	���α׷� ���� : 
			���� : �׸� ���߱� ����
				- Ű����κ��� �Է¹޾� ������ ��� �� �����׸��� �Լ��� ���� ������ ���ҵ� ������ ���
				- ���ҵ� ������ �Ϻθ� �������� ��� ������ ����ó���� �����ϰ� Ű����κ��� ����� �������� ������ ����.
*/

#include <opencv2/opencv.hpp>	//opencv �Լ����� ����ϱ����� ��ó����
#include <stdlib.h>			
#include <time.h>			
#include <stdio.h>			

using namespace cv;		
using namespace std;

void devideImage(Mat img, int num);		// ������ �����ϴ� devideImage�Լ�
void rectImage(Mat img, int num);		// ���ҵ� ���󿡼� ������ ����ó�� ȿ���� �����ϰ� ������ ���� rectImage�Լ�

int main(int argc, char* argv[]) {			// main�Լ�			
	Mat image, otherImage;		// ���� ������ ������ image, otherImage ���� ����
	srand(time(NULL));						
	int num = 0;							// Ű����κ��� �Է¹޾� ���� ������ ������ num ���� �� 0���� �ʱ�ȭ

	printf("���� ����\n1.����\t\t2.��ѱ�\t3.�м���\t4.������\n��ȣ �Է� : ");	// Ű����κ��� � ������ ������� ���
	scanf_s("%d", &num);			// �Է¹��� ������ num�� ����

	switch (num) {						// num�� ���� ���� finalProject�� �ִ� ���� ���
	case 1:
		image = imread("����.jpg");			// imread�� ������ image�� ����
		break;
	case 2:
		image = imread("��ѱ�.jpg");
		break;
	case 3:
		image = imread("�м���.jpg");
		break;
	case 4:
		image = imread("������.jpg");
		break;
	default :
		break;
	}
	if (!image.data) {			// ������ ���� ���ϸ� -1�� ��ȯ�ϰ� ���α׷� ����
		printf("������ ���� ���� ������ �߻��߽��ϴ�...\n");
		return -1;
	}
	imshow("Basie image", image);	// swtich���� ���� ����� image ������ ���
	waitKey(1);						// 1�ʵ��� Ű�� �Է¹ޱ⸦ ��ٸ�����, ��� ����Ǵ°� ����������� ���� �ڵ带 �����Ѵ�. 
	
	otherImage = image.clone();		// clone�Լ��� ���� otherImage������ �����Ѵ�. �޸𸮸� ���� �����ٴ� ������ �ִ�.

	printf("�����ú���\n1.4����(2X2)\n2.9����(3X3)\n3.16����(4X4)\n��ȣ �Է� : ");		// ������ ����ҷ� ������ ���
	scanf_s("%d", &num);				// �ٽ� �Է¹��� ������ num�� ���� ����
	putText(otherImage, "Devide Image", cvPoint(0, 50), FONT_HERSHEY_SIMPLEX, 1, cvScalar(0, 0, 0), 2);
									// num�� ���� ���� ���ҵ� �̹����� putText�Լ��� ���� Devide Image��� ���ڸ� ����Ѵ�.
	destroyAllWindows();			// Basic imageâ�� �ݾ��ش�.

	if (num > 0 && num < 4)				// ������ �� �̿��� ���� �Է��ϸ� ���α׷� ����
		devideImage(otherImage, num + 1);
	else {
		printf("1,2,3�� ���� ��ȣ�� �Է��� ������ ����մϴ�.");
		return -1;
	}
	
	imshow("divide image", otherImage);	// Devide Image��� �۾��� ���� ������ ���� �̹����� ���ҽ�Ų ������ ���
	waitKey(1);							

	rectImage(image, num + 1);	// ���ҵ� �̹����� ���� ������ ���� rectImage �Լ�
										
	system("pause");			// rectImage �Լ��� ���� ���� ����� ������ â�� ���߰�
	destroyAllWindows();		// � ���̵� �ԷµǸ� destoryAllWindow()�Լ��� ���� ��� ������ �ݴ´�.
	return 0;					// 0�� ��ȯ
}

void devideImage(Mat img, int num) {		// ������ ����� img�� ����ڰ� �Է��� ���� num�� �Ű������� ���´�.
	int i, j;								// �ݺ������� ����� ����
	for (i = 1; i < num; i++) {			// ������ ������ ������ �׸��� ���� ��ŭ ���������� ���� �ݺ���
		for (j = 1; j < num; j++) {
			line(img, cvPoint(img.cols / num * i, 0), cvPoint(img.cols / num * i, img.rows), CvScalar(100, 200, 100), 2);
			line(img, cvPoint(0, img.rows / num * j), cvPoint(img.cols, img.rows / num * j), CvScalar(100, 200, 100), 2);
		
			/*	line(�̹���,X,Y,����,������ ũ��)
				line�Լ��� ���� X���� ���� Y���� � �������� �ũ�⸸ŭ ������ �׸���.
				cvPoint�Լ��� ���� ��ġ�� �������ְ�, ������ ������� �������� �������� ������ �׸���.
				
				CvScalar(Bule,Grenn,Red) BGR�� �����ΰͿ� �����Ѵ�.*/
		}
	}
}

void rectImage(Mat img, int num) {				// �̹����� �Ϻκ��� �߶� ������ ����ó���� �����ϴ� rectImage�Լ�
	Mat partImg;					// ���ҵ� ������ ���� ������ partImg ����
	int ranCol = rand() % num;		// 0 ~ num-1�� ���� ���´�.
	int	ranRow = rand() % num;		// ������ ��� ���� �������� ������ ������ �κ��� �����Ѵ�.
	int ranChange = rand() % 11;	// 0~10�� ���� ���´�. �� ������ ���� ������ ����ó���� �����Ѵ�.
	int x, y;						// Ű����κ��� ���� ������ ���� x,y����

	Rect rect(img.cols / num * ranCol, img.rows / num * ranRow, img.cols / num, img.rows / num);		
	/*	���ɿ����� �ڸ��� rect(X,Y,W,H)
		X�� ��ġ���� W��ŭ, Y�� ��ġ H��ŭ �ڸ���.
		���⼭�� ���ҵ� ���� ���� N������� �������� ���󿡼� �� ���� N������� �������� ������� �̹����� �߶��ش�.
	*/
	
	img = img(rect);				// �׷��� �߸� �̹�����img�� �����Ѵ�.
	partImg = img.clone();			// ���ҵ� ������ ���� �����Ѵ�, ���� ���� ��¿� ����Ѵ�.

	printf("�Ϻ� ������ �ٲ�����ϴ�!!!\n");		// ���� ����
	switch (ranChange){					// ranChange ���� ���� ���ҵ� ���� ������ ����ó���� �����Ѵ�.
										// � ����ó���� ����Ǿ������� ������ְ� swtich���� ����������.
	case 0 :
		img += CvScalar(100, 100, 100);						// ���� 100��ŭ �������ν� �̹����� ��� �Ѵ�.
		printf("��Ʈ: ������ ��� ���� �����߽��ϴ�!\n");
		break;
	case 1:
		img -= CvScalar(100, 100, 100);						// ���� 100��ŭ �������ν� �̹����� ��Ӱ� �Ѵ�.
		printf("��Ʈ: ������ ��� ���� �����߽��ϴ�!\n");
		break;
	case 2:
		img = CvScalar(255, 255, 255) - img;		// CvScalar�� ���� ������� ������� ������ ����ŭ ���־� �̹����� ������ �����Ѵ�.
		printf("��Ʈ: ������ �������� ���� �ٲ�����ϴ�!\n");
		break;
	case 3:
		img -= CvScalar(0, 255, 255);			// �̹������� �Ķ����� �����ϰ� �ٸ� ������ 0���� ����� �̹����� �Ķ����� �и��Ѵ�.
		printf("��Ʈ: ������ BLUE���� �и��Ǿ����ϴ�!\n");
		break;
	case 4:
		img -= CvScalar(255, 0, 255);			// �̹������� �ʷϻ��� �����ϰ� �ٸ� ������ 0���� ����� �̹����� �ʷϻ��� �и��Ѵ�.
		printf("��Ʈ: ������ GREEN���� �и��Ǿ����ϴ�\n");
		break;
	case 5:
		img -= CvScalar(255, 255,0 );			// �̹������� �������� �����ϰ� �ٸ� ������ 0���� ����� �̹����� �������� �и��Ѵ�.
		printf("��Ʈ: ������ RED���� �и��Ǿ����ϴ�n");
		break;
	case 6:
		flip(partImg, img, 1);						// filp�Լ��� ���� ������ ������Ų��. 1�� ������ �¿������ ����ȴ�.
		printf("��Ʈ: ������ �¿�� �����Ǿ����ϴ�!\n");
		break;
	case 7:
		flip(partImg, img, 0);						// �������, 0�� ������ ���Ϲ����� ����ȴ�.
		printf("��Ʈ: ������ ���Ϸ� �����Ǿ����ϴ�!\n");
		break;
	case 8:
		flip(partImg, img, -1);						// �������, -1�� ������ �����¿������ ����ȴ�.
		printf("��Ʈ: ������ �����¿�� �����Ǿ����ϴ�!\n");
		break;
	case 9:
		GaussianBlur(partImg, img, Size(0, 0), 3);	// ����þ� ����, ������ ���� ������ ����� ���̰� �������ش�.
		printf("��Ʈ: ���� ����þ� ���Ͱ� ����Ǿ����ϴ�!\n");
		break;
	case 10:
		Laplacian(partImg, img, 0);		// Laplacian�� ���� ���� ���ö�þ��� �������ش�.
		printf("��Ʈ: ���� ���ö�þ� ���Ͱ� ����Ǿ����ϴ�!\n");
		break;
	default:	
		break;
	}

	imshow("change!!", img);			// swtich���� ���� �ٲ� img�� ����Ѵ�.
	waitKey(1);

	printf("�� ������ ��ü ������ ���� ���° ���� ���° ���ϱ��? :\n");		// ���� ������ ��� ���� ���ߴ� ����
	scanf_s("%d %d", &x, &y);		// �Է¹��� �� ������ x,y�� ����

	if ((y == ranCol + 1) && (x == ranRow + 1))			// �̹����� ��� ���� �ݴ�� �Ǿ������� �����Ѵ�.
		printf("�����Դϴ�!\n");
	else
		printf("�����Դϴ�..\n%d���� %d���̾����ϴ�..\n",ranRow+1,ranCol+1);

	printf("�Ϻ� ������ �����˴ϴ�.\n");		
	imshow("partImage", partImg);			// ó�� �и��� ������ �����ߴ� partImg����� ���� ����ȭ�� ���
	waitKey(1);
}