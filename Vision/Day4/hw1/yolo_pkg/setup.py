import os

from glob import glob
from setuptools import find_packages, setup


package_name = 'yolo_pkg'


setup(
    name=package_name,
    version='0.0.0',

    packages=find_packages(),

    data_files=[
        (
            'share/ament_index/resource_index/packages',
            ['resource/' + package_name]
        ),

        (
            'share/' + package_name,
            ['package.xml']
        ),

        # launch 파일 설치
        (
            os.path.join(
                'share',
                package_name,
                'launch'
            ),
            glob('launch/*.launch.py')
        ),

        # yaml 파일 설치
        (
            os.path.join(
                'share',
                package_name,
                'config'
            ),
            glob('config/*.yaml')
        ),
    ],

    install_requires=[
        'setuptools'
    ],

    zip_safe=True,

    maintainer='kdh',
    maintainer_email='kdh@example.com',

    description='YOLO26 ROS2 Object Detection',

    license='TODO',

    entry_points={
        'console_scripts': [
            'yolo_node = yolo_pkg.yolo_node.yolo_node:main',
        ],
    },
)