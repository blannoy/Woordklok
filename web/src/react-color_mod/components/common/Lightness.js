
import React, { Component, PureComponent } from 'react'
import reactCSS from 'reactcss'
import * as lightness from '../../helpers/lightness'

export class Lightness extends (PureComponent || Component) {
  componentWillUnmount() {
    this.unbindEventListeners()
  }

  handleChange = (e, pointerIndex) => {
    const change = lightness.calculateChange(e, this.props.direction, (pointerIndex===0?this.props.hsl:this.props.hsl2), this.container)
    if (change) {
      typeof this.props.onChange === 'function' && this.props.onChange(change, e)
    }
  }

  handleMouseDown = (e, pointerIndex) => {
    this.handleChange(e, pointerIndex)
    window.addEventListener('mousemove', this.handleMouseMove(e,pointerIndex))
    window.addEventListener('mouseup', this.handleMouseUp)
  }

  handleMouseMove =(e,pointerIndex) => {
    console.log(e.type+" - "+pointerIndex+ " - "+this.props.hsl.l)
    this.handleChange(e,pointerIndex);
  }
  handleMouseUp = () => {
    this.unbindEventListeners()
  }

  unbindEventListeners() {
    window.removeEventListener('mousemove', this.handleChange)
    window.removeEventListener('mouseup', this.handleMouseUp)
  }

  render() {
    const { direction = 'horizontal' } = this.props

    const styles = reactCSS({
      'default': {
        lightness: {
          absolute: '0px 0px 0px 0px',
          borderRadius: this.props.radius,
          boxShadow: this.props.shadow,
        },
        container: {
          padding: '0 2px',
          position: 'relative',
          height: '100%',
          borderRadius: this.props.radius,
        },
        pointer: {
          position: 'absolute',
          left: `${ (this.props.hsl.l * 100) }%`,
        },
        pointer2: {
          position: 'absolute',
          left: `${ ((this.props.hsl2!==undefined?this.props.hsl2.l:0)* 100) }%`,
          top: '50%',
        },
        slider: {
          marginTop: '1px',
          width: '4px',
          borderRadius: '1px',
          height: '8px',
          boxShadow: '0 0 2px rgba(0, 0, 0, .6)',
          background: '#fff',
          transform: 'translateX(-2px)',
        },
      },
      'vertical': {
        pointer: {
          left: '0px',
          top: `${ ((100 - this.props.hsl.l) * 100) }%`,
        },
        pointer2: {
          left: '0px',
          top: `${ ((100 - (this.props.hsl2!==undefined?this.props.hsl2.l:0)) * 100) }%`,
        },
      },
    }, { vertical: direction === 'vertical' })

    return (
      <div style={ styles.lightness }>
        <div
          className={ `lightness-${ direction }` }
          style={ styles.container }
          ref={ container => this.container = container }
          onMouseDown={ e => this.handleMouseDown(e, 0) }
          onTouchMove={ e => this.handleChange(e, 0) }
          onTouchStart={ e => this.handleChange(e, 0) }
        >
          <style>{ `
                      .lightness-horizontal {
                        background: linear-gradient(to right,hsl(${ Math.round(this.props.hsl.h) }, ${ Math.round(this.props.hsl.s) }%,0%), hsl(${ Math.round(this.props.hsl.h) }, ${ Math.round(this.props.hsl.s) }%,100%));
                        background: -webkit-linear-gradient(to right,hsl(0, 100%,0%), hsl(${ Math.round(this.props.hsl.h) }, 100%, 50%), hsl(360, 100%,100%));
                      }

                      .lightness-vertical {
                        background: linear-gradient(to top, hsl(0, 100%,0%), hsl(${ Math.round(this.props.hsl.h) }, 100%, 50%), hsl(360, 100%,100%));
                        background: -webkit-linear-gradient(to top,hsl(0, 100%,0%), hsl(${ Math.round(this.props.hsl.h) }, 100%, 50%), hsl(360, 100%,100%));
                      }
          ` }</style>
          <div style={ styles.pointer } onMouseDown={ e => this.handleMouseDown(e, 0) } >
            { this.props.pointer ? (
              <this.props.pointer { ...this.props } />
            ) : (
              <div style={ styles.slider } />
            ) }
          </div>
          {this.props.hsl2 && (
          <div style={ styles.pointer2 } onMouseDown={ e => this.handleMouseDown(e, 1) }>
            { this.props.pointer ? (
              <this.props.pointer { ...this.props } />
            ) : (
              <div style={ styles.slider } />
            ) }
          </div>)}
        </div>
      </div>
    )
  }
}

export default Lightness
