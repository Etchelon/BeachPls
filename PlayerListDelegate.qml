import QtQuick 2.2

Rectangle {
	id: delegate

	readonly property color selectedColor: "#FF1BA1E2"
	readonly property color unselectedColor: "lightgray"
	property bool selected: ListView.isCurrentItem

	anchors {
		left: parent.left
		right: parent.right
	}
	height: 80

	color: selected ? delegate.selectedColor : delegate.unselectedColor

	Text {
		anchors {
			fill: parent
			margins: 10
		}

		text: delegate.ListView.view.model[index].name
		font.pixelSize: height * 0.5
		font.bold: true
		verticalAlignment: Text.AlignVCenter
	}

	MouseArea {
		anchors.fill: parent
		onClicked: delegate.ListView.view.currentIndex = index;
	}
}
