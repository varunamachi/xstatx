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
        if(cnt >= 0.98 *  xAxis.tickCount ) {
            chart.scrollRight( scroll )
        }
        series.append(cnt, Vam.Stat.getCPUTemparature() )
        core1.append(cnt, Vam.Stat.getCoreTemparature(0) )
        core2.append(cnt, Vam.Stat.getCoreTemparature(1) )
        core3.append(cnt, Vam.Stat.getCoreTemparature(2) )
        core4.append(cnt, Vam.Stat.getCoreTemparature(3) )
    }

    visible: true
    width: 640
    height: 480
    Timer {
        id: mainTimer
        interval: 300;
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
//            animationOptions: ChartView.SeriesAnimations
            anchors.fill: parent
            antialiasing: true
            ValueAxis {
                id: xAxis
                visible: false
                min: 0
                max: 100
                tickCount: 100
            }
            ValueAxis{
                id: yAxis
                min: 0
                max: 100
                titleText: "Temp [&deg;C]"
            }
            SplineSeries {
                id: core1
                axisX: xAxis
                axisY: yAxis
                width: 1
                name: "Core 0"
            }
            SplineSeries {
                id: core2
                axisX: xAxis
                axisY: yAxis
                width: 1
                name: "Core 1"
            }
            SplineSeries {
                id: core3
                axisX: xAxis
                axisY: yAxis
                width: 1
                name: "Core 2"
            }
            SplineSeries {
                id: core4
                axisX: xAxis
                axisY: yAxis
                width: 1
                name: "Core 3"
            }
            SplineSeries {
                id: series
                axisX: xAxis
                axisY: yAxis
                width: 3
                name: "CPU"
                color: "green"
            }
        }
    }
}
