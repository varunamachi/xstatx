import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Templates 2.0
import QtCharts 2.0

import vam 1.0 as Vam

Window {
    property double prev: 0
    property real cnt: 0
    function update() {
        var offset = (xAxis.max - xAxis.min) / xAxis.tickCount
        var scroll = chart.plotArea.width / xAxis.tickCount
        cnt = cnt + offset
        if(cnt >= 0.9 *  xAxis.tickCount ) {
            chart.scrollRight( scroll )
        }
        var cur = Vam.Stat.cpuTemp();
        series.append(cnt, cur )
    }

    visible: true
    width: 640
    height: 480
    Timer {
        id: mainTimer
        interval: 1000;
        running: true;
        repeat: true
        onTriggered: update()
    }
    Rectangle {
        id: colRect
        anchors.fill: parent
        MouseArea {
            anchors.fill: parent
            onClicked: {
                colRect.color = "red"
                update()
            }
        }
        ChartView {
            id: chart
//            animationOptions: ChartView.AllAnimations
            anchors.fill: parent
            antialiasing: true
            ValueAxis {
                id: xAxis
                visible: false
                min: 0
                max: 50
                tickCount: 50
            }
            ValueAxis{
                id: yAxis
                min: 0
                max: 100
                titleText: "Temperature [&deg;C]"
            }
            LineSeries {
                id: series
                axisX: xAxis
                axisY: yAxis
                name: "CPU Temprature"
            }
        }
    }
}
